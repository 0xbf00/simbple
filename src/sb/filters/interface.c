//
// Created by Jakob Rieck on 07.02.18.
//
#include "data.h"
#include "filters.h"
#include "mem.h"
#include "types.h"

#include <assert.h>
#include <misc/misc.h>
#include <sb/error.h>
#include <string.h>

#include <scheme_interpreter/scheme-private.h>

pointer handle_generic_filter(scheme *sc, pointer args) {
  const size_t nargs = scheme_arguments.count(sc, args);
  assert(nargs >= 1 && "Not enough arguments to generic filter handler.");

  pointer first_arg = scheme_arguments.next(sc, &args);
  assert(is_string(first_arg) && "First argument is not a string.");

  // The filter name is always the first argument, because
  // we made sure of that (see below in register_handlers)
  const char *filter_name = string_value(first_arg);

  const filter_info_t *const filter_info = filter_info_for_name(filter_name);
  assert(filter_info);

  sbpl_filter_t *result = filter_create(filter_info);

  // Work on any arguments the filter may have, ignoring filters
  // that can handle more complex arguments, such as the network-filters
  // for now.
  for (size_t i = 1; i < nargs; ++i) {
    pointer arg = scheme_arguments.next(sc, &args);

    // Check whether we are dealing with a placeholder!
    bool argument_is_placeholder =
        is_string(arg) &&
        filter_is_valid_named_argument(filter_info, string_value(arg));

    if (filter_accepts_type(filter_info, SB_VALUE_TYPE_INTEGER) ||
        filter_accepts_type(filter_info, SB_VALUE_TYPE_BITFIELD)) {
      if (argument_is_placeholder) {
        // Get the value behind the placeholder
        const uint16_t arg_val =
            filter_value_for_named_argument(filter_info, string_value(arg));

        result = filter_add_argument(
            result, (sbpl_filter_argument_t){.alias = string_value(arg),
                                             .argument.as_int = arg_val,
                                             .type = PRIMITIVE});

      } else if (!(is_number(arg) && is_integer(arg))) {
        free(result);
        return error_blob(sc, "Filter argument is of invalid type.",
                          LOCATION_UNKNOWN, SBPL_FILTER);
      } else {
        const uint64_t arg_val = ivalue(arg);
        const named_argument *alias =
            filter_named_argument_for_value(filter_info, arg_val);

        result = filter_add_argument(
            result,
            (sbpl_filter_argument_t){.alias = alias ? alias->name : NULL,
                                     .argument.as_int = arg_val,
                                     .type = PRIMITIVE});
      }
    } else if (filter_accepts_type(filter_info, SB_VALUE_TYPE_BOOL)) {
      if (!is_bool(sc, arg)) {
        free(result);
        return error_blob(sc, "Filter argument is of invalid type.",
                          LOCATION_UNKNOWN, SBPL_FILTER);
      }

      bool val = bool_value(sc, arg);
      const uint64_t arg_val = val ? 1 : 0;
      const named_argument *alias =
          filter_named_argument_for_value(filter_info, arg_val);

      result = filter_add_argument(
          result, (sbpl_filter_argument_t){.alias = alias ? alias->name : NULL,
                                           .argument.as_int = arg_val,
                                           .type = BOOLEAN});
    } else if (filter_accepts_type(filter_info, SB_VALUE_TYPE_NETWORK)) {
      // Normally, the order of the arguments is important. We ignore this here,
      // because we assume the profiles we get are well-formed.

      if (argument_is_placeholder) {
        const uint16_t arg_val =
            filter_value_for_named_argument(filter_info, string_value(arg));

        result = filter_add_argument(
            result, (sbpl_filter_argument_t){.alias = string_value(arg),
                                             .argument.as_int = arg_val,
                                             .type = PRIMITIVE});
      } else if (is_string(arg)) {
        result = filter_add_argument(
            result,
            (sbpl_filter_argument_t){.alias = NULL,
                                     .argument.as_str = string_value(arg),
                                     .type = STRING});
      } else if (is_number(arg) && is_integer(arg)) {
        const uint64_t arg_val = ivalue(arg);
        const named_argument *alias =
            filter_named_argument_for_value(filter_info, arg_val);

        result = filter_add_argument(
            result,
            (sbpl_filter_argument_t){.alias = alias ? alias->name : NULL,
                                     .argument.as_int = arg_val,
                                     .type = PRIMITIVE});
      } else {
        free(result);
        return error_blob(sc, "Network filter argument is of invalid type",
                          LOCATION_UNKNOWN, SBPL_FILTER);
      }
    } else {
      // We treat everything else as simple strings.
      // Make sure the current filter actually accepts one of these types.
      assert(filter_accepts_type(filter_info, SB_VALUE_TYPE_PATTERN_LITERAL) ||
             filter_accepts_type(filter_info, SB_VALUE_TYPE_PATTERN_PREFIX) ||
             filter_accepts_type(filter_info, SB_VALUE_TYPE_PATTERN_SUBPATH) ||
             filter_accepts_type(filter_info, SB_VALUE_TYPE_REGEX) ||
             filter_accepts_type(filter_info, SB_VALUE_TYPE_STRING));

      if (!is_string(arg)) {
        free(result);
        return error_blob(
            sc,
            "Argument to filter expecting string like object is not a string.",
            LOCATION_UNKNOWN, SBPL_FILTER);
      } else {
        result = filter_add_argument(
            result,
            (sbpl_filter_argument_t){.alias = NULL,
                                     .argument.as_str = string_value(arg),
                                     .type = STRING});
      }
    }
  }

  return scheme_blob(sc, result, SBPL_FILTER);
}

static bool redirect_filter(scheme *sc, const filter_info_t *const filter) {
  char *filter_str = NULL;
  asprintf(&filter_str, "\"%s\"", filter->name);

  assert(filter_str && "Unable to allocate memory for filter redirection.");

  bool status = scheme_support.redirect_function(sc, filter->name,
                                                 "filter-generic", filter_str);

  free(filter_str);
  return status;
}

/**
 * Checks whether all arguments are filters.
 */
pointer is_filter(scheme *sc, pointer args) {
  const size_t n_args = scheme_arguments.count(sc, args);

  for (size_t i = 0; i < n_args; ++i) {
    pointer arg = scheme_arguments.next(sc, &args);
    if (!is_blob(arg)) {
      return sc->F;
    }
    if (blob_info(arg).tag != SBPL_FILTER &&
        blob_info(arg).tag != SBPL_META_FILTER) {
      return sc->F;
    }
  }

  return sc->T;
}

/**
 * Registers derived filters, if any derived filters for the current filter are
 * available.
 * @param sc The current scheme context.
 * @param base_filter Filter that might have derived filters.
 * @return True, whether each derived filter could be registered successfully.
 */
static bool register_derived_filters(scheme *sc,
                                     const filter_info_t *const base_filter) {
  bool status = true;

  // Register the %-prefix derived filter.
  if (filter_accepts_type(base_filter, SB_VALUE_TYPE_PATTERN_SUBPATH)) {
    char *derived_name = NULL;
    asprintf(&derived_name, "%s-prefix", base_filter->name);
    if (!derived_name)
      return false;

    // Most of the information about the derived filter is
    // identical to the information from its base filter.
    const filter_info_t *derived_filter = derived_filter_add(
        (filter_info_t){.name = derived_name,
                        .category = base_filter->category,
                        .argument_type = base_filter->argument_type,
                        .prerequisite = base_filter->prerequisite,
                        .named_arguments = base_filter->named_arguments});

    free(derived_name);

    status &= (derived_filter != NULL) && redirect_filter(sc, derived_filter);
  }

  // Register a derived filter with a "-subpath" suffix, or with "path" replaced
  // with "subpath", if "path" is in the filter name
  if (filter_accepts_type(base_filter, SB_VALUE_TYPE_PATTERN_REGEX)) {
    char *derived_name = NULL;

    const size_t space_required = strlen(base_filter->name) + 10;

    WITH_MEM_BLOCK(space_required, derived_name, {
      strcpy(derived_name, base_filter->name);

      char *path_loc = strstr(derived_name, "path");
      if (path_loc == NULL) {
        // Simply append "-subpath"
        strlcat(derived_name, "-subpath", space_required);
      } else {
        // "path" is in the filtername. Fixup the name
        memmove(path_loc + 3, path_loc, strlen(path_loc) + 1);
        path_loc[0] = 's';
        path_loc[1] = 'u';
        path_loc[2] = 'b';
      }

      const filter_info_t *derived_filter = derived_filter_add(
          (filter_info_t){.name = derived_name,
                          .category = base_filter->category,
                          .argument_type = base_filter->argument_type,
                          .prerequisite = base_filter->prerequisite,
                          .named_arguments = base_filter->named_arguments});

      status &= (derived_filter != NULL) && redirect_filter(sc, derived_filter);
    });
  }

  // Register %-regex derived filter
  if (filter_accepts_type(base_filter, SB_VALUE_TYPE_REGEX)) {
    char *derived_name = NULL;
    asprintf(&derived_name, "%s-regex", base_filter->name);
    if (!derived_name)
      return false;

    // Most of the information about the derived filter is
    // identical to the information from its base filter.
    const filter_info_t *derived_filter = derived_filter_add(
        (filter_info_t){.name = derived_name,
                        .category = base_filter->category,
                        .argument_type = base_filter->argument_type,
                        .prerequisite = base_filter->prerequisite,
                        .named_arguments = base_filter->named_arguments});

    // derived_filter_add duplicates the passed-in name
    free(derived_name);

    status &= (derived_filter != NULL) && redirect_filter(sc, derived_filter);
  }

  return status;
}

/**
 * Registers a specific filter with the Scheme runtime.
 * @param sc The current scheme context
 * @param filter The filter to register. This specific filter does not need
 * to be in the pre-existing list of filters. This is done in order to
 * support derived filters.
 * @return The return value indicates whether or not the operation was
 * successful.
 */
static bool register_filter(scheme *sc, const filter_info_t *const filter) {
  return redirect_filter(sc, filter) && register_derived_filters(sc, filter);
}

/**
 * PRIVATE
 * Registers filter named arguments to a string representation of themselves.
 * @param sc The current scheme context
 * @param filter The filter for which to register the named arguments
 */
static void register_filter_arguments(scheme *sc,
                                      const filter_info_t *const filter) {
  for (const named_argument *current_argument = filter->named_arguments;
       current_argument != NULL && current_argument->name != NULL;
       current_argument++) {
    scheme_support.register_object(sc, current_argument->name,
                                   mk_string(sc, current_argument->name));
  }
}

/**
 * Registers handlers for each filter defined in filter_infos.
 *
 * @param sc The current scheme context
 */
static void register_filter_handlers(scheme *sc,
                                     filter_data_provider data_provider) {
  /*
   * High-Level Overview
   * -------------------
   * There are many different filters (> 50), so
   * it would be very cumbersome to write a dedicated function
   * for each and every one.
   * Instead, we start by defining only one generic handler,
   * which is then called by each filter in turn. To do this,
   * we use the Scheme runtime to dynamically redirect each filter
   * to this generic handler (see below)
   */

  // Register generic filter handler
  scheme_support.register_function(
      sc, (foreign_function_t){.symname = "filter-generic",
                               .foreign_function = handle_generic_filter});

  scheme_support.register_function(
      sc, (foreign_function_t){.symname = "sbpl-filter?",
                               .foreign_function = is_filter});

  // Register available filters with runtime
  filters_install(data_provider);

  // Register all named arguments, but not to their (integer) value,
  // but to another string. This simplifies handling (see above)
  for (size_t i = 0; i < filters_get_count(); ++i) {
    const filter_info_t *const current_filter = filter_info_for_idx(i);
    if (!current_filter)
      continue;

    register_filter_arguments(sc, current_filter);
  }

  /*
   * Redirect each filter to the filter-generic function,
   * while also supplying an additional argument that
   * identifies the specific filter that was invoked.
   */
  for (size_t i = 0; i < filters_get_count(); ++i) {
    const filter_info_t *const current_filter = filter_info_for_idx(i);
    if (!current_filter)
      continue;

    bool filter_registed = register_filter(sc, current_filter);
    assert(filter_registed && "Failed to redirect filter.");
  }
}

const sbpl_filter_interface_t filters = {.register_handlers =
                                             register_filter_handlers};