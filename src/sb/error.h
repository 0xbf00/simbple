//
// Created by Jakob Rieck on 15.02.18.
//

#ifndef SBPL_ERROR_H
#define SBPL_ERROR_H

#include <stdint.h>

#include "blob_handling.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOCATION_UNKNOWN (-1)

typedef struct scheme scheme;

#pragma pack(1)
typedef struct {
  /**
   * A human-readable description of the error.
   */
  const char *description;
  /**
   * The location in the source code where
   * the error originated or LOCATION_UNKNOWN, if no such
   * information is available
   */
  int32_t location;
  /**
   * The type of SBPL primitive that resulted
   * in the error. Allowed options are all except
   * SBPL_ERROR.
   */
  enum sbpl_tag tag;
} sbpl_error_t;
#pragma pack()

/**
 * Create a TinySCHEME blob describing an error.
 *
 * @param sc Current scheme context. Needed for creating blobs.
 * @param err_desc The human-readable description of the error
 * @param src_loc The location in the source code that resulted in the error
 * @param tag The kind of SBPL primitive resulting in an error
 * @return Pointer to a blob structure that contains all the needed information
 * and is structured as follows: sbpl_error_t|err_desc
 * This way, a single free can be used to free all resources. (See also
 * modifier_convert_to_blob and related functions in modifiers.{h|c})
 */
pointer error_blob(scheme *sc, const char *err_desc, int32_t src_loc,
                   enum sbpl_tag tag);

/**
 * Free an error object. Used internally by the scheme runtime.
 * @param err The object to free
 */
void error_free(sbpl_error_t *err);

#ifdef __cplusplus
}
#endif

#endif // SBPL_ERROR_H
