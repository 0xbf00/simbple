/**
 * Copyright (C) Jakob Rieck 2018
 *
 * Meta-filters are higher-level filters that are employed by Apple
 * in their Sandbox language. These filters can be used to apply logical
 * predicates to other filters and to combine filters in powerful ways.
 *
 * Meta-filters can be nested. There are three kinds of meta-filters:
 *  - require-not (logical not)
 *  - require-all (logical and)
 *  - require-any (logical any)
 *
 * A sample usage would be the following
 *
 * (allow file-read*
 *      (require-any
 *          (subpath "/usr/bin")
 *          (subpath "/usr/local/bin")
 *          (subpath "/bin")))
 *
 * that would grant file-read permissions only iff the file requested resides
 * at a path in any one of these directories.
 *
 * In libsandbox, meta-filters are referred to simply as "conditions", and all
 * filters that are not meta-filters are referred to as "filters".
 */
#ifndef SBPL_META_FILTERS_H
#define SBPL_META_FILTERS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scheme scheme;

// Public interface
typedef struct {
  void (*register_handlers)(scheme *);
} sbpl_meta_filter_interface_t;

extern const sbpl_meta_filter_interface_t meta_filters;

#ifdef __cplusplus
}
#endif

#endif // SBPL_META_FILTERS_H