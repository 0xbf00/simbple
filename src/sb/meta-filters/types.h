//
// Created by Jakob Rieck on 11.02.18.
//
#ifndef SBPL_META_FILTERS_TYPES_H
#define SBPL_META_FILTERS_TYPES_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct cell;
typedef struct cell *pointer;

enum meta_filter_type { METAFILTER_NOT, METAFILTER_ANY, METAFILTER_ALL };

/**
 * Learning from mistakes in the standard filter case, memory
 * management for the meta-filters is but an afterthought. Memory
 * might leak, but it is no big deal, because the runtime of the
 * program is limited and the memory usage negligible.
 */
#pragma pack(1)
typedef struct {
  enum meta_filter_type type;

  size_t filters_count;
  pointer *filters;
} sbpl_meta_filter_t;
#pragma pack()

/**
 * Frees a meta-filter. Used by the underlying Scheme runtime.
 * @param filter The meta filter to free.
 */
void meta_filter_free(sbpl_meta_filter_t *filter);

#ifdef __cplusplus
}
#endif

#endif // SBPL_META_FILTERS_TYPES_H
