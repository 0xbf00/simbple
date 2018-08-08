//
// Created by Jakob Rieck on 07.02.18.
//
#ifndef SBPL_FILTERS_DATA_H
#define SBPL_FILTERS_DATA_H

#include <stdbool.h>

#include "types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Function to install / register a new set of filters
 * with the current runtime
 */
void filters_install(filter_data_provider provider);

/**
 * Returns the number of sandbox filters (normal and derived filters)
 * that are currently known.
 */
const size_t filters_get_count();
const size_t derived_filters_get_count();

/**
 * Adds a new derived filter to the internal list of derived filters.
 *
 * This function takes ownership of all memory references passed in.
 *
 * @param filter_info The filter_info structure to register dynamically
 * @return Pointer to the new filter_info_t argument or NULL, if the operation
 * did not succeed.
 */
filter_info_t *derived_filter_add(const filter_info_t filter);

/**
 * Lookup filter name for a filter index. Note that 0 is not a valid filter
 * index. Returns NULL on error.
 */
const char *filter_name_for_idx(const size_t filter_idx);

/**
 * Return the filter_info_t structure for a given index or filter name.
 * Note that not all argument values are treated as valid. For instance, 0 is
 * not a valid filter index.
 *
 * @return A pointer to the desired filter_info_t structure or NULL, if the
 * argument is invalid.
 */
const filter_info_t *const filter_info_for_idx(size_t filter_idx);
const filter_info_t *const filter_info_for_name(const char *filter_name);

/**
 * Lookup raw value for a named argument.
 *
 * @pre filter_is_valid_named_argument(filter, alias)
 * @param filter filter that has a certain named argument
 * @param alias alias for the named argument
 * @return raw value of the named argument
 */
uint16_t filter_value_for_named_argument(const filter_info_t *filter,
                                         const char *alias);

/**
 * Lookup if the supplied alias is a valid named argument for the filter.
 *
 * @param filter filter to identify named arguments
 * @param alias alias to check against a number of named arguments the filter
 * might have
 * @return pointer to the alias string or NULL, if the alias does not denote a
 * valid named argument.
 */
const char *filter_is_valid_named_argument(const filter_info_t *filter,
                                           const char *alias);

/**
 * Reverse lookup a named argument from a raw value.
 *
 * @param filter filter that has the named arguments to search through
 * @param value value for which to search for the name.
 * @return pointer to named_argument structure or NULL, if no such structure was
 * found.
 */
const named_argument *
filter_named_argument_for_value(const filter_info_t *filter, uint16_t value);

/**
 * Check whether a filter's argument is case-sensitive.
 *
 * @pre filter_idx != FILTER_NOT_FOUND
 * @param filter_idx index of the filter
 * @return boolean value indicating whether the argument shall be treated
 * case sensitive or not.
 */
bool filter_argument_is_case_sensitive(size_t filter_idx);

/**
 * Check whether the argument type is complex.
 * Complex arguments are all types that are passed around by pointer, including
 * strings and other objects. Integers and boolean values are not complex
 * objects
 *
 * @param arg_type argument type to look at.
 * @return boolean value indicating whether the argument type is complex
 */
bool filter_argument_is_complex(enum filter_argument_type arg_type);

/**
 * Check whether the specified filter accepts values of the specified type.
 *
 * @param filter information about the filter at hand
 * @param type type to check
 * @return boolean value indicating whether the specified filter accepts the
 * type or not
 */
bool filter_accepts_type(const filter_info_t *filter,
                         enum filter_argument_type type);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_FILTERS_DATA_H