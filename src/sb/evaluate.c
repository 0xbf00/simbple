//
// Created by Jakob Rieck on 15.05.18.
//

#include "evaluate.h"

#include "actions.h"
#include "filters/filters.h"
#include "modifiers.h"
#include "operations/operations.h"

#include "dyn_params.h"
#include "rule_base.h"

#include <assert.h>
#include <misc/misc.h>
#include <scheme_interpreter/scheme-private.h>

static int error_occurred = false;

/**
 * Stub function that does nothing but is required for the Scheme interpreter
 */
pointer stub_func(scheme *sc, pointer args) { return sc->NIL; }

pointer handle_error(scheme *sc, pointer args) {
  size_t nargs = scheme_arguments.count(sc, args);

  pointer msg = scheme_arguments.next(sc, &args);
  assert(is_string(msg) && "No error message provided to error handler.");
  fprintf(stderr, "[Error]: %s", string_value(msg));

  if (nargs >= 2) {
    pointer sym = scheme_arguments.next(sc, &args);
    if (is_symbol(sym)) {
      fprintf(stderr, " %s\n", symname(sym));
    } else {
      fprintf(stderr, "\n");
    }
  } else {
    fprintf(stderr, "\n");
  }

  // Print remaining arguments
  for (size_t i = 2; i < nargs; ++i) {
    pointer arg = scheme_arguments.next(sc, &args);

    if (is_string(arg)) {
      printf("Argument %zu is string: %s\n", i, string_value(arg));
    } else if (is_number(arg)) {
      printf("Argument %zu is number: %f\n", i, rvalue(arg));
    }
  }

  error_occurred = true;

  return sc->NIL;
}

/**
 * Evaluate an SBPL input profile.
 *
 * @param profile The profile to evaluate
 * @param container_metadata The container metadata to use for evaluation
 * @param should_patch Whether to patch the generated rules to enable logging
 * for all rules
 * @param format The output format (either OUTPUT_JSON or OUTPUT_SCHEME)
 * @return Generated rules as a string.
 */
const char *sb_evaluate(const char *profile, const char *container_metadata,
                        enum target_platform platform, bool should_patch,
                        enum output_format format) {
  // Make sure the container file is actually sensible.
  if (validate_container_file(container_metadata)) {
    fprintf(stderr, "[Err]: Not a valid container metadata file.\n");
    return NULL;
  }

  scheme *sc = scheme_init_new();

  scheme_support.load_file(
      sc, program_relative_path("/../../src/scm/apple-init.scm"));

  // Register error handler
  scheme_define(sc, sc->global_env, sc->ERROR_HOOK,
                mk_foreign_func(sc, handle_error));

  scheme_support.register_function(
      sc, (foreign_function_t){"%version-1", stub_func});
  scheme_support.register_function(
      sc, (foreign_function_t){"%notify-file-open", stub_func});
  scheme_support.register_function(
      sc, (foreign_function_t){"split-string", split_string});
  scheme_support.register_function(
      sc, (foreign_function_t){"regex-quote", regex_quote});

  if (container_metadata) {
    register_params(sc, container_metadata);
    register_entitlements(sc, container_metadata);
  }

  // Register handlers
  modifiers.register_handlers(sc);
  filters.register_handlers(sc, filters_for_platform(platform));
  meta_filters.register_handlers(sc);
  operations.register_handlers(sc, operations_for_platform(platform));
  actions.register_handlers(sc);

  scheme_support.load_file(sc,
                           program_relative_path("/../../src/scm/sbpl.scm"));
  scheme_support.load_file(
      sc, program_relative_path("/../../src/scm/sbpl_prelude.scm"));
  scheme_support.load_file(sc,
                           program_relative_path("/../../src/scm/sbpl_v1.scm"));

  scheme_support.load_file(sc, profile);

  // Additional sandbox snippets are loaded after loading the main profile.
  if (container_metadata)
    load_additional_sandbox_snippets(sc, container_metadata);

  // Patching: Make sure each allow operation is logged.
  if (should_patch) {
    patch_report();
  }

  const char *output =
      (format == OUTPUT_JSON) ? rule_base_dump_json() : rule_base_dump_scheme();

  scheme_deinit(sc);

  return output;
}