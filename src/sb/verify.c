//
// Created by Jakob Rieck on 13.06.18.
//

#include "verify.h"
#include "dyn_params.h"
#include <memory.h>
#include <stdio.h>

const size_t N_COMPILATION_TRIES = 10;

struct sb_profile {
  unsigned int type;
  void *data;
  size_t data_len;
};

struct sb_profile *sandbox_compile_string(const char *profile, void *params,
                                          char **error);

/**
 * Verify that a generated plaintext sandbox profile is equivalent
 * to the sandbox profile compiled by the sandbox. This is done
 * by compiling the generated profile and comparing it to the existing
 * compiled version.
 *
 * @param generated_profile Full generated profile
 * @param container_metadata Path to container metadata
 * @return VERIFICATION_SUCCESSFUL, if the profiles are semantically
 * equivalent.
 */
enum verification_result sb_verify(const char *generated_profile,
                                   const char *container_metadata) {
  enum verification_result result = VERIFICATION_FAILED;

  // Load precompiled version from disk
  struct data_buffer *existing_profile =
      extract_compiled_profile(container_metadata);
  if (!existing_profile)
    return VERIFICATION_FAILED;

  /*
   * Compile the generated SBPL
   *
   * Starting with macOS High Sierra, sandbox compilation is no longer
   * a fully deterministic process. Therefore, in order to verify the resulting
   * profiles by comparing compilation results, it may be necessary to re-try
   * compilation a number of times. In my tests, no more than 5 subsequent tries
   * were needed. 10 therefore is a conservative number.
   */
  for (size_t i = 0; i < N_COMPILATION_TRIES; ++i) {
    char *error = NULL;
    struct sb_profile *compiled_profile =
        sandbox_compile_string(generated_profile, NULL, &error);
    if (error) {
      fprintf(stderr, "[Err]: Compilation failed: %s\n", error);

      result = VERIFICATION_INVALID_PROFILE;
    } else {
      // Compare existing and recompiled versions
      if ((existing_profile->length == compiled_profile->data_len) &&
          (memcmp(compiled_profile->data, existing_profile->data,
                  existing_profile->length) == 0)) {

        result = VERIFICATION_SUCCESSFUL;
      }
    }

    if (result == VERIFICATION_SUCCESSFUL ||
        result == VERIFICATION_INVALID_PROFILE)
      break;
  }

  free(existing_profile->data);
  free(existing_profile);

  return result;
}
