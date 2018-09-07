/**
 * Simple CLI interface to the libsbpldump component.
 *
 * This programs can be used to convert JSON dumps to valid
 * SBPL after the fact. This might come in handy if you want
 * to modify the JSON output of `simbple` programmatically,
 * but afterwards would like to have the corresponding SBPL.
 */
#include "convert.h"

#include <cstdio>
#include <cstring>
#include <cassert>

#define EXIT_INVALID_USAGE 2

void print_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s input.json\n", program_name);
    fprintf(stderr, "    Specify \"-\" as input.json to read from stdin.\n");
}

/**
 * Reads a file f entirely into memory and returns
 * the buffer. The caller is responsible for cleaning up
 * the memory
 *
 * @param f The file to read.
 * @return Buffer containing file contents.
 */
void *read_file(FILE *f, int add_terminating_nul)
{
    size_t bufsize = 4 * 1024 + 1;
    size_t buf_pos = 0;

    auto *buffer = static_cast<unsigned char *>(malloc(bufsize));
    if (!buffer)
        return nullptr;

    while (true) {
        const size_t current_capacity = bufsize - buf_pos;
        size_t bytes_read = fread(buffer+buf_pos, 1, current_capacity, f);

        if (bytes_read < current_capacity) {
            if (add_terminating_nul) {
                *(buffer + buf_pos + bytes_read) = '\0';
            }
            break;
        } else {
            bufsize *= 2;

            buffer = (unsigned char *)realloc(buffer, bufsize);
            buf_pos += bytes_read;
        }
    }

    return buffer;
}

int main(int argc, char *argv[]) {
    const char *program_name = argv[0];

    if (argc != 2) {
        print_usage(program_name);
        return EXIT_INVALID_USAGE;
    } else if (strcmp("--help", argv[1]) == 0) {
        print_usage(program_name);
        return EXIT_SUCCESS;
    }

    FILE *from = nullptr;

    if (strcmp(argv[1], "-") == 0) {
        // read from stdin
        from = stdin;
    } else {
        from = fopen(argv[1], "rb");
    }

    assert(from != nullptr);

    auto *contents = static_cast<const char *>(read_file(from, true));
    auto *output   = sandbox_rules_dump_scheme(contents);

    puts(output);
}