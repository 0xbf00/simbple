#ifndef MISC_H
#define MISC_H

#include "argument_handling.h"
#include "filesystem.h"
#include "regex.h"
#include "scheme_support.h"
#include "string.h"
#include "os_support.h"

#include <stdlib.h>

/** Memory and pointer related functions and macros **/

/**
 * Automatically free a resource after executing code.
 */
#define WITH_MEM_BLOCK(size, storage, code)                                    \
  do {                                                                         \
    (storage) = calloc(1, size);                                               \
    if (storage) {                                                             \
      code;                                                                    \
      free(storage);                                                           \
      (storage) = NULL;                                                        \
    }                                                                          \
  } while (0)

#define PTR_ADVANCE_NBYTES(ptr, nbytes) ((void *)((uint8_t *)ptr + nbytes))

// Make sure to only supply alignments that are powers of two!
#define ALIGN_SIZE(size, alignment)                                            \
  do {                                                                         \
    if (((size) & (alignment - 1)) != 0) {                                     \
      (size) = ((size) + (alignment)) & ~(alignment - 1);                      \
    }                                                                          \
  } while (0)

#endif // MISC_H