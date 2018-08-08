/**
 * Copyright (C) Jakob Rieck, 2018
 *
 * The purpose of this source file is to encapsulate all the required
 * memory-related functions that are used to create, modify and copy the output
 * structures that are emitted from the filter handlers.
 */
#ifndef SBPL_FILTERS_MEM_H
#define SBPL_FILTERS_MEM_H

#include "types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Create a new filter object.
 *
 * @param filter_info information about the filter type
 * @return filter object carrying associated information.
 */
sbpl_filter_t *filter_create(const filter_info_t *const filter_info);

/**
 * Add an argument to an existing filter
 *
 * @param filter filter object to update
 * @param arg argument to add to the filter
 * @return returns the filter input.
 */
sbpl_filter_t *filter_add_argument(sbpl_filter_t *filter,
                                   const sbpl_filter_argument_t arg);

/**
 * Retrieves an argument from a filter object.
 *
 * @param filter filter from which to extract an argument
 * @param arg_idx index of the argument
 * @return pointer to the argument or NULL, if the argument is invalid or could
 * not be extracted.
 */
sbpl_filter_argument_t *filter_get_argument(sbpl_filter_t *filter,
                                            const size_t arg_idx);

/**
 * Free a filter. Used by the scheme runtime.
 *
 * @param filter filter to free
 */
void filter_free(sbpl_filter_t *filter);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_FILTERS_MEM_H