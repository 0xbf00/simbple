//
// Created by Jakob Rieck on 08.02.18.
//

#include <misc/misc.h>

#include "mem.h"
#include "types.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Create a new filter object.
 *
 * @param filter_info information about the filter type
 * @return filter object carrying associated information.
 */
sbpl_filter_t *filter_create(const filter_info_t *const filter_info) {
  sbpl_filter_t *mem = calloc(1, sizeof(sbpl_filter_t));
  assert(mem && "Unable to get memory for filter.");

  mem->filter_name = strdup(filter_info->name);
  mem->n_arguments = 0;
  mem->arguments = NULL;

  return mem;
}

/**
 * Add an argument to an existing filter
 *
 * @param filter filter object to update
 * @param arg argument to add to the filter
 * @return returns the filter input.
 */
sbpl_filter_t *filter_add_argument(sbpl_filter_t *filter,
                                   const sbpl_filter_argument_t arg) {
  assert(arg.type != 0 && "Invalid argument type supplied.");

  filter->n_arguments += 1;
  filter->arguments = realloc(
      filter->arguments, sizeof(sbpl_filter_argument_t) * filter->n_arguments);
  assert(filter->arguments && "Unable to allocate memory for new argument.");

  sbpl_filter_argument_t *new_arg = &filter->arguments[filter->n_arguments - 1];

  new_arg->type = arg.type;
  new_arg->alias = arg.alias ? strdup(arg.alias) : NULL;

  if (arg.type == STRING) {
    new_arg->argument.as_str = strdup(arg.argument.as_str);
  } else {
    new_arg->argument.as_int = arg.argument.as_int;
  }

  return filter;
}

/**
 * Retrieves an argument from a filter object.
 *
 * @param filter filter from which to extract an argument
 * @param arg_idx index of the argument
 * @return pointer to the argument or NULL, if the argument is invalid or could
 * not be extracted.
 */
sbpl_filter_argument_t *filter_get_argument(sbpl_filter_t *filter,
                                            const size_t arg_idx) {
  assert(arg_idx < filter->n_arguments);

  return &filter->arguments[arg_idx];
}

/**
 * Free a filter. Used by the scheme runtime.
 *
 * @param filter filter to free
 */
void filter_free(sbpl_filter_t *filter) {
  // Free resources associated with filter arguments
  for (size_t i = 0; i < filter->n_arguments; ++i) {
    sbpl_filter_argument_t *arg = filter_get_argument(filter, i);
    free(arg->alias);

    if (arg->type == STRING) {
      free(arg->argument.as_str);
    }
  }

  // Free resources of filter
  free(filter->arguments);
  free(filter->filter_name);

  // Free the filter itself
  free(filter);
}