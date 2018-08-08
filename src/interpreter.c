#include <stdlib.h>
#include <unistd.h>

#include <platform_data/platforms.h>

#include <misc/misc.h>
#include <sb/evaluate.h>
#include <sb/verify.h>

#include <argp.h>

const char *argp_program_version = "0.9";

struct program_arguments {
  const char *profile;
  const char *outfile;

  const char *container_metadata;

  enum target_platform platform;

  bool json_output;
  bool scheme_output;

  bool patch;
  bool should_verify;
};

static int parse_opt(int key, char *arg, struct argp_state *state) {
  struct program_arguments *args = state->input;

  switch (key) {
  case 'p':
    args->profile = arg;
    break;
  case 'o':
    args->outfile = arg;
    break;
  case 333:
    // Parse platform
    args->platform = platform_from_string(arg);
    break;
  case 666:
    args->patch = true;
    break;
  case 777:
    args->should_verify = true;
    break;
  case OUTPUT_JSON:
    args->json_output = true;
    args->scheme_output = false;
    break;
  case OUTPUT_SCHEME:
    args->scheme_output = true;
    args->json_output = false;
    break;
  case ARGP_KEY_ARG: {
    if (args->container_metadata)
      argp_failure(state, 1, 0, "At most one container metadata file allowed.");
    if (!file_exists(arg))
      argp_failure(state, 1, 0, "Not a valid Container metadata file.");

    args->container_metadata = arg;
    break;
  }
  case ARGP_KEY_END: {
    // Sanity check arguments
    if (!file_exists(args->profile))
      argp_failure(state, 1, 0, "Profile file does not exist.");
    if (args->container_metadata && !file_exists(args->container_metadata))
      argp_failure(state, 1, 0, "Container metadata does not exist.");
    if (!args->container_metadata)
      argp_failure(state, 1, 0, "No container metadata supplied.");
    if (args->platform == PLATFORM_UNKNOWN)
      argp_failure(state, 1, 0, "Incorrect platform specified.");
    if (state->arg_num > 1)
      argp_failure(state, 1, 0, "At most one container metadata file allowed.");
  }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  // Argument parsing
  struct argp_option options[] = {
      {"profile", 'p', "PROFILE", 0,
       "Base profile to evaluate. Defaults to application.sb profile."},
      {"output", 'o', "FILE", 0, "Output file"},
      {"platforms", 333, "PLATFORM", 0, list_target_platforms()},
      {0, 0, 0, 0, "Output formats:", 2},
      {"json", OUTPUT_JSON, 0, 0, "Output as JSON"},
      {"scheme", OUTPUT_SCHEME, 0, 0, "Output as SCHEME / SBPL"},
      {0, 0, 0, 0, "Misc options:", 3},
      {"patch", 666, 0, 0, "Patch the output profile to log all statements."},
      {"verify", 777, 0, 0, "Verify semantic correctness of generated results"},
      {0}};

  struct argp argp = {options, parse_opt, "CONTAINER_METADATA",
                      "Evaluate a SBPL (+ Scheme) profile\v"
                      "The output is a simplified SBPL profile that can be "
                      "analysed, modified and compiled as is."};

  // Default parameters: application.sb file and scheme output
  struct program_arguments arguments = {
      .profile = "/System/Library/Sandbox/Profiles/application.sb",
      .scheme_output = true,
      .platform = platform_get_default(),
  };

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // Evaluate the input
  const char *output = sb_evaluate(
      arguments.profile, arguments.container_metadata, arguments.platform,
      arguments.patch, arguments.json_output ? OUTPUT_JSON : OUTPUT_SCHEME);

  if (!output)
    return EXIT_FAILURE;

  // Optionally verify results
  if (arguments.should_verify) {
    enum verification_result result =
        sb_verify(output, arguments.container_metadata);
    if (result != VERIFICATION_SUCCESSFUL) {
      return EXIT_FAILURE;
    }
  }

  // Print out the results
  FILE *out = arguments.outfile ? fopen(arguments.outfile, "w") : stdout;

  fprintf(out, "%s\n", output);
  fclose(out);

  return EXIT_SUCCESS;
}
