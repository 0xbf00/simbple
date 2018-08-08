//
// Created by Jakob Rieck on 16.02.18.
//

#include "blob_handling.h"
#include <assert.h>

#include "sb/filters/mem.h"
#include "sb/meta-filters/mem.h"
#include "sb/operations/mem.h"

#include "sb.h"
#include <scheme_interpreter/scheme.h>

static void free_blob(void *blob, size_t tag) {
  switch (tag) {
  case SBPL_ERROR:
    return error_free(blob);
  case SBPL_MODIFIER:
    return modifier_free(blob);
  case SBPL_FILTER:
    return filter_free(blob);
  case SBPL_META_FILTER:
    return meta_filter_free(blob);
  case SBPL_ACTION:
    return action_free(blob);
  case SBPL_OPERATION:
    return operation_free(blob);
  default:
    assert(false && "Free not yet implemented.");
  }
}

/**
 * Make a scheme blob.
 * @param sc The current scheme context
 * @param ptr The pointer to store in the blob
 * @param tag The type of data stored
 * @return Scheme's pointer denoting the blob stored
 */
pointer scheme_blob(scheme *sc, void *ptr, enum sbpl_tag tag) {
  return mk_blob(sc, tag, ptr, 0, free_blob);
}