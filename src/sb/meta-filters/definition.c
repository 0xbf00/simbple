//
// Created by Jakob Rieck on 11.02.18.
//

#include "definition.h"
#include "types.h"

#include <assert.h>
#include <misc/misc.h>
#include <sb/error.h>

#include <scheme_interpreter/scheme-private.h>

/**
 * Implement the "require-not" filter. Not meant to be called directly.
 * @param sc The current scheme context
 * @param args The arguments from the scheme runtime
 * @return A new value to be passed to other functions
 */
pointer require_not(scheme *sc, pointer args) {
  /*
   * The "require-not" filter expects a single argument of type filter.
   * (Revealed through manual probing of the original functionality)
   */

  size_t nargs = scheme_arguments.count(sc, args);
  if (nargs != 1) {
    return error_blob(
        sc, "Incorrect number of arguments to \"require-not\" filter.",
        LOCATION_UNKNOWN, SBPL_META_FILTER);
  }

  pointer arg = scheme_arguments.next(sc, &args);
  if (!is_blob(arg)) {
    return error_blob(
        sc,
        "Incorrect argument type for \"require-not\" filter. Expected blob.",
        LOCATION_UNKNOWN, SBPL_META_FILTER);
  }

  blob_info_t arg_info = blob_info(arg);
  if (!(arg_info.tag == SBPL_FILTER || arg_info.tag == SBPL_META_FILTER)) {
    return error_blob(sc, "Unsupported object in argument blob.",
                      LOCATION_UNKNOWN, SBPL_META_FILTER);
  }

  /*
   * Allocate memory for sub-filters and copy pointer to single sub-filter.
   */
  pointer *sub_filters = calloc(
      1 /* require-not filter has a single sub-filter */, sizeof(pointer));
  assert(sub_filters && "Unable to allocate memory for output.");

  sub_filters[0] = arg;

  sbpl_meta_filter_t *not_filter = calloc(1, sizeof(sbpl_meta_filter_t));
  assert(not_filter && "Unable to allocate memory for output.");

  not_filter->filters_count = 1;
  not_filter->type = METAFILTER_NOT;
  not_filter->filters = sub_filters;

  return scheme_blob(sc, not_filter, SBPL_META_FILTER);
}

/* Internal helper function, called by require_any and require_all */
static pointer generic_require_many(scheme *sc, pointer args,
                                    enum meta_filter_type meta_filter) {
  /**
   * All meta-filters that take more than a single argument
   * ("require-any") and ("require-all") require all arguments to
   * be either filters or meta-filters themselves.
   */
  const size_t nargs = scheme_arguments.count(sc, args);
  if (nargs < 1) {
    return error_blob(
        sc,
        (meta_filter == METAFILTER_ANY)
            ? "Invalid number of arguments supplied to require-any function."
            : "Invalid number of arguments supplied to require-all function.",
        LOCATION_UNKNOWN, SBPL_META_FILTER);
  }

  pointer *sub_filters = calloc(nargs, sizeof(pointer));
  assert(sub_filters && "Unable to allocate memory for output.");

  for (int i = 0; i < nargs; ++i) {
    pointer arg = scheme_arguments.next(sc, &args);

    if (!is_blob(arg)) {
      free(sub_filters);

      return error_blob(
          sc, "Incorrect argument type for meta-filter. Expected blob.",
          LOCATION_UNKNOWN, SBPL_META_FILTER);
    }

    blob_info_t bi = blob_info(arg);
    if (!(bi.tag == SBPL_FILTER || bi.tag == SBPL_META_FILTER)) {
      free(sub_filters);

      return error_blob(sc, "Unsupported object in argument blob.",
                        LOCATION_UNKNOWN, SBPL_META_FILTER);
    }

    sub_filters[i] = arg;
  }

  sbpl_meta_filter_t *result = calloc(1, sizeof(sbpl_meta_filter_t));
  assert(result && "Unable to allocate memory for output.");

  result->type = meta_filter;
  result->filters_count = nargs;
  result->filters = sub_filters;

  return scheme_blob(sc, result, SBPL_META_FILTER);
}

/**
 * Implement the "require-any" filter. Not meant to be called directly.
 * @param sc The current scheme context
 * @param args The arguments from the scheme runtime
 * @return A new value to be passed to other functions
 */
pointer require_any(scheme *sc, pointer args) {
  return generic_require_many(sc, args, METAFILTER_ANY);
}

/**
 * Implement the "require-all" filter. Not meant to be called directly.
 * @param sc The current scheme context
 * @param args The arguments from the scheme runtime
 * @return A new value to be passed to other functions
 */
pointer require_all(scheme *sc, pointer args) {
  return generic_require_many(sc, args, METAFILTER_ALL);
}