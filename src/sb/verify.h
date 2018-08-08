//
// Created by Jakob Rieck on 13.06.18.
//

#ifndef SBPL_VERIFY_H
#define SBPL_VERIFY_H

#include <stdbool.h>

enum verification_result {
  VERIFICATION_SUCCESSFUL = 0,
  VERIFICATION_INVALID_PROFILE,
  VERIFICATION_FAILED
};

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
                                   const char *container_metadata);

#endif // SBPL_VERIFY_H
