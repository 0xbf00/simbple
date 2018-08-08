//
// Created by Jakob Rieck on 16.02.18.
//

#include "mem.h"
#include <scheme_interpreter/scheme.h>

/**
 * Frees a meta-filter. Used by the underlying Scheme runtime.
 * @param filter The meta filter to free.
 */
void meta_filter_free(sbpl_meta_filter_t *filter) {
  // The individual sub filters themselves will be
  // freed by the runtime automatically.
  free(filter->filters);

  free(filter);
}