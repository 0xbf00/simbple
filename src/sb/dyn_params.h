//
// Created by Jakob Rieck on 20.02.18.
//

#ifndef SBPL_DYN_PARAMS_H
#define SBPL_DYN_PARAMS_H

#include <stdlib.h>

typedef struct scheme scheme;

struct data_buffer {
  void *data;
  size_t length;
};

void register_params(scheme *sc, const char *filename);
void register_entitlements(scheme *sc, const char *filename);
void load_additional_sandbox_snippets(scheme *sc, const char *filename);

/**
 * Extracts the compiled profile bytecode from the container metadata
 * file
 *
 * @param filename The filename of the container file from which to read the
 * bytecode
 * @return NULL on failure. Else a structure of type data_buffer that contains
 * the compiled profile bytecode. The caller is responsible for releasing the
 * memory.
 */
struct data_buffer *extract_compiled_profile(const char *filename);

/**
 * Validate a container metadata file.
 * Check that all required keys are available and that the file is a valid plist
 * file.
 *
 * @param filename Filename of the container metadata to check.
 * @return 0 on success
 */
int validate_container_file(const char *filename);

#endif // SBPL_DYN_PARAMS_H
