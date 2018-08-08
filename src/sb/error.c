//
// Created by Jakob Rieck on 15.02.18.
//

#include "error.h"
#include "sb.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

/**
 * Create a TinySCHEME blob describing an error.
 *
 * @param err_desc The human-readable description of the error
 * @param src_loc The location in the source code that resulted in the error
 * @param tag The kind of SBPL primitive resulting in an error
 * @return Pointer to a blob structure that contains all the needed information
 * and is structured as follows: sbpl_error_t|err_desc
 * This way, a single free can be used to free all resources. (See also
 * modifier_convert_to_blob and related functions in modifiers.{h|c})
 */
pointer error_blob(scheme *sc, const char *err_desc, int32_t src_loc,
                   enum sbpl_tag tag) {
  assert(err_desc && (tag != SBPL_ERROR) &&
         "Invalid error_blob usage: No string or wrong tag provided.");

  sbpl_error_t *err = calloc(1, sizeof(sbpl_error_t));
  assert(err && "Unable to allocate memory.");

  err->location = src_loc;
  err->tag = tag;
  err->description = strdup(err_desc);

  return scheme_blob(sc, err, SBPL_ERROR);
}

void error_free(sbpl_error_t *err) {
  free(err->description);
  free(err);
}