/**
 * Copyright (C) Jakob Rieck, 2018
 *
 * The purpose of this source file is to encapsulate all the required
 * memory-related functions that are used to create, modify and copy the output
 * structures that are emitted from the meta filters handlers.
 */
#ifndef SBPL_META_FILTERS_MEM_H
#define SBPL_META_FILTERS_MEM_H

#include "types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Frees a meta-filter. Used by the underlying Scheme runtime.
 * @param filter The meta filter to free.
 */
void meta_filter_free(sbpl_meta_filter_t *filter);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_META_FILTERS_MEM_H