//
// Created by Jakob Rieck on 07.02.18.
//

#include <assert.h>
#include <misc/misc.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "types.h"

struct filters {
  const filter_info_t *regular_filters;
  filter_info_t *derived_filters;

  // Number of regular filters
  size_t n_regular;
  // Number of derived filters
  size_t n_derived;

  filter_accept_type_func acceptor_func;
};

static struct filters *sb_filters = NULL;

void filters_install(filter_data_provider provider) {
  assert(sb_filters == NULL && "Filters already installed.");

  sb_filters = calloc(sizeof(struct filters), 1);
  assert(sb_filters && "Failed to allocate memory for filters backing store");

  sb_filters->n_regular =
      provider(&sb_filters->regular_filters, &sb_filters->acceptor_func);
}

const size_t filters_get_count() {
  assert(sb_filters != NULL && "Filters not yet installed.");
  return sb_filters->n_regular;
}

const size_t derived_filters_get_count() {
  assert(sb_filters != NULL && "Filters not yet installed.");
  return sb_filters->n_derived;
}

const char *filter_name_for_idx(const size_t filter_idx) {
  if (filter_idx > filters_get_count())
    return NULL;

  return sb_filters->regular_filters[filter_idx].name;
}

/**
 * Adds a new derived filter to the internal list of derived filters.
 *
 * This function takes ownership of all memory references passed in.
 *
 * @param filter_info The filter_info structure to register dynamically
 * @return Pointer to the new filter_info_t argument or NULL, if the operation
 * did not succeed.
 */
filter_info_t *derived_filter_add(const filter_info_t filter) {
  assert(sb_filters != NULL && "Filters not yet installed.");

  // Resize backing store
  sb_filters->derived_filters =
      realloc(sb_filters->derived_filters,
              sizeof(filter_info_t) * (sb_filters->n_derived + 1));
  assert(sb_filters->derived_filters != NULL &&
         "Unable to resize derived filters backing store.");

  filter_info_t *new = &sb_filters->derived_filters[sb_filters->n_derived];

  // Take ownership of new information
  new->name = strdup(filter.name);
  new->category = strdup(filter.category);
  new->argument_type = filter.argument_type;
  new->prerequisite = filter.prerequisite;
  // named-arguments, just like the category, are in all cases
  // existing data structures that don't need to be copied because
  // they reside in memory anyway and cannot be freed.
  new->named_arguments = filter.named_arguments;

  sb_filters->n_derived += 1;

  return new;
}

/**
 * Return the filter_info_t structure for a given (valid index)
 *
 * @param filter_idx The index of the filter. Note that 0 is treated as not a
 * valid filter.
 * @return A pointer to the desired filter_info_t structure or NULL, if invalid.
 */
const filter_info_t *const filter_info_for_idx(const size_t filter_idx) {
  if (filter_idx == 0)
    return NULL;

  if (filter_idx > filters_get_count())
    return NULL;

  return &sb_filters->regular_filters[filter_idx];
}

/**
 * Return the filter_info_t structure for a given filter name.
 * @param filter_name The name of the filter
 * @return Pointer to the filter_info_t entry or NULL, if not found
 */
const filter_info_t *const filter_info_for_name(const char *filter_name) {
  for (size_t i = 0; i < filters_get_count(); ++i) {
    if (sb_filters->regular_filters[i].name == NULL)
      continue;

    if (strcmp(sb_filters->regular_filters[i].name, filter_name) == 0) {
      return &sb_filters->regular_filters[i];
    }
  }

  // Also search derived filters for the name
  for (size_t i = 0; i < derived_filters_get_count(); ++i) {
    if (sb_filters->derived_filters[i].name != NULL) {
      if (strcmp(sb_filters->derived_filters[i].name, filter_name) == 0) {
        return &sb_filters->derived_filters[i];
      }
    }
  }

  return NULL;
}

/**
 * Check whether the argument to the filter is case-sensitive.
 * @param filter_idx The index of the filter (FILTER_NOT_FOUND not allowed!)
 * @return True, iff the argument is case-sensitive, else false.
 */
bool filter_argument_is_case_sensitive(const size_t filter_idx) {
  /*
   * The ith bit describes whether or not the ith filter receives an argument
   * that is case-sensitive
   */
  const uint64_t case_sensitive_bitfield =
      0b1100000000000100111101011001110011100000010011101000;

  assert(filter_idx != FILTER_NOT_FOUND);
  assert(filter_idx < filters_get_count());

  return ((case_sensitive_bitfield >> (filter_idx - 1)) & 0x1) ? true : false;
}

/**
 * Check whether the argument type is of a complex type, i.e. a string
 * @param arg_type The argument type to look at.
 * @return True, iff the argument type is "complex", else false
 */
bool filter_argument_is_complex(enum filter_argument_type arg_type) {
  return (arg_type != SB_VALUE_TYPE_BITFIELD &&
          arg_type != SB_VALUE_TYPE_BOOL && arg_type != SB_VALUE_TYPE_INTEGER);
}

/**
 * Check whether the specified filter accepts the specified type.
 * This function is different for sierra, high sierra, and thus defined
 * in the platform specific data!
 */
bool filter_accepts_type(const filter_info_t *const filter,
                         enum filter_argument_type type) {
  assert(sb_filters != NULL && "Filters not yet initialized.");
  assert(sb_filters->acceptor_func != NULL &&
         "Filter type checking func not installed.");

  return sb_filters->acceptor_func(filter, type);
}

/**
 * Returns the numeric value for a given named argument, provided that
 * argument exists (precondition).
 * @pre filter_is_valid_named_argument(filter, alias)
 * @param filter The filter for which to query the named argument
 * @param alias The named argument to query
 * @return The value of the named argument
 */
uint16_t filter_value_for_named_argument(const filter_info_t *const filter,
                                         const char *alias) {
  assert(filter_is_valid_named_argument(filter, alias) &&
         "Filter has no such named argument");

  const named_argument *named_arguments = filter->named_arguments;
  while (named_arguments->name != NULL) {
    if (strcmp(named_arguments->name, alias) == 0) {
      return named_arguments->value;
    }
    named_arguments++;
  }

  assert(false && "Pre-condition was violated. This should never happen.");
  return 0;
}

/**
 * Some filters have named arguments that can be supplied. Check whether
 * the supplied alias is a valid argument for that filter
 * @pre alias != NULL
 * @param filter The filter for which to check the alias
 * @param alias The alias to check against a number of possible arguments for
 * the filter
 * @return True, iff alias is a valid named argument for filter.
 */
const char *filter_is_valid_named_argument(const filter_info_t *const filter,
                                           const char *alias) {
  if (!alias)
    return NULL;

  if (filter->named_arguments == NULL)
    return NULL;

  const named_argument *named_arguments = filter->named_arguments;
  while (named_arguments->name != NULL) {
    if (strcmp(named_arguments->name, alias) == 0) {
      // Found match
      return named_arguments->name;
    }

    named_arguments++;
  }

  return NULL;
}

const named_argument *
filter_named_argument_for_value(const filter_info_t *filter,
                                const uint16_t value) {
  if (filter->named_arguments == NULL)
    return NULL;

  const named_argument *named_arguments = filter->named_arguments;
  while (named_arguments->name != NULL) {
    if (named_arguments->value == value)
      return named_arguments;
    named_arguments++;
  }

  return NULL;
}