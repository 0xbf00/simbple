#include "modifiers.h"
#include "blob_handling.h"
#include "sb.h"
#include <assert.h>
#include <memory.h>
#include <misc/misc.h>
#include <scheme_interpreter/scheme-private.h>

// Most of the information in here is gleaned from
// ___define_action_block_invoke_2
// -> ___define_action_block_invoke
// ----> ___sb_sbpl_parser_init_scheme_block_invoke
// --------> _sb_sbpl_parser_new
const modifier_info_t modifier_infos[] = {
    // the first entry (see disassembly) is empty
    {
        .modifier_name = NULL,
        .applies_to_action = ACTION_NONE,
        .action_mask = 0,
        .action_flag = 0,
    },
    {// has one argument
     .modifier_name = "send-signal",
     .applies_to_action = ACTION_ALL,
     .action_mask = 0,
     .action_flag = 0},
    {.modifier_name = "report",
     .applies_to_action = ACTION_ALLOW,
     .action_mask = 0x4,
     .action_flag = 0x4},
    {.modifier_name = "no-report", // AKA no-log
     .applies_to_action = ACTION_DENY,
     .action_mask = 0x4,
     .action_flag = 0x0},
    {.modifier_name = "no-sandbox",
     .applies_to_action = ACTION_ALLOW,
     .action_mask = 0x8,
     .action_flag = 0x8},
    {.modifier_name = "grant",
     .applies_to_action = ACTION_ALLOW,
     .action_mask = 0x2,
     .action_flag = 0x2},
    {.modifier_name = "rootless",
     .applies_to_action = ACTION_DENY,
     .action_mask = 0x10,
     .action_flag = 0x10},
    {.modifier_name = "no-times",
     .applies_to_action = ACTION_ALLOW,
     .action_mask = 0x20,
     .action_flag = 0x20},
    {// has one argument
     .modifier_name = "default-sandbox",
     .applies_to_action = ACTION_ALLOW,
     .action_mask = 0x0,
     .action_flag = 0x0}};

/**
 * Return the number of currently known modifiers.
 * @return The number of currently known modifiers.
 */
size_t modifiers_get_count() {
  return sizeof(modifier_infos) / sizeof(*modifier_infos);
}

/**
 * Private helper function to find an entry in the list of modifiers info
 * without relying on hardcoded indices.
 * @param name
 * @return
 */
size_t modifier_idx_for_name(const char *name) {
  assert(name != NULL && "Invalid parameter for modifier_idx_for_name");
  for (size_t i = 0; i < sizeof(modifier_infos) / sizeof(*modifier_infos);
       ++i) {
    if (modifier_infos[i].modifier_name &&
        strcmp(modifier_infos[i].modifier_name, name) == 0) {
      return i;
    }
  }

  assert(false && "Invalid modifier name supplied.");
  return 0; // Never reached
}

/**
 * Create a copy of the modifier.
 * @param input The structure to duplicate. All contents are duplicated.
 * @return Copy of the modifier
 */
static void *modifier_full_copy(sbpl_modifier_t input) {
  sbpl_modifier_t *out = calloc(1, sizeof(sbpl_modifier_t));
  assert(out && "Unable to allocate memory for modifier.");

  out->argument_type = input.argument_type;
  out->modifier_name = strdup(input.modifier_name);

  if (out->argument_type == MODIFIER_ARGUMENT_STR) {
    out->argument.string_arg = strdup(input.argument.string_arg);
  } else if (out->argument_type == MODIFIER_ARGUMENT_INT) {
    out->argument.int_arg = input.argument.int_arg;
  }

  return out;
}

/**
 * Frees a sbpl_modifier_t object. Exported to be used by the scheme runtime.
 * @param modifier The modifier to free. All resources, as well as the modifier
 * are freed.
 */
void modifier_free(sbpl_modifier_t *modifier) {
  free(modifier->modifier_name);
  if (modifier->argument_type == MODIFIER_ARGUMENT_STR) {
    free(modifier->argument.string_arg);
  }

  free(modifier);
}

/**
 * Converts a modifier to a blob structure.
 *
 * @param sc The scheme context.
 * @param modifier The structure to convert to a scheme blob
 * @return Pointer to a blob that can be returned from scheme foreign functions.
 */
pointer modifier_convert_to_blob(scheme *sc, sbpl_modifier_t modifier) {
  sbpl_modifier_t *modifier_copy = modifier_full_copy(modifier);
  assert(modifier_copy != NULL && "Unable to create full copy of modifier.");

  return scheme_blob(sc, modifier_copy, SBPL_MODIFIER);
}

/**
 * Generic handler for modifiers that receive no arguments and that
 * thus have a very similar structure.
 *
 * @param sc The current scheme context
 * @param args The argument list. Should be empty.
 * @param modifier_name The name of the modifier that has no arguments
 * @return A structure describing the modifier
 */
pointer handle_no_arguments_generic(scheme *sc, pointer args,
                                    const char *modifier_name) {
  size_t nargs = scheme_arguments.count(sc, args);

  if (nargs != 0) {
    fprintf(stderr, "\"%s\" received %zu arguments, but needs %d arguments\n",
            modifier_name, nargs, 0);

    return error_blob(sc, "Incorrect number of args", LOCATION_UNKNOWN,
                      SBPL_MODIFIER);
  }

  return modifier_convert_to_blob(
      sc,
      (sbpl_modifier_t){.modifier_name =
                            modifier_infos[modifier_idx_for_name(modifier_name)]
                                .modifier_name,
                        .argument_type = MODIFIER_ARGUMENT_NONE});
}

pointer handle_send_signal(scheme *sc, pointer args) {
  // send signal receives a single argument
  size_t nargs = scheme_arguments.count(sc, args);

  if (nargs != 1) {
    fprintf(stderr,
            "\"send-signal\" received %zu arguments, but needs %d argument\n",
            nargs, 1);

    return error_blob(sc, "Incorrect number of args", LOCATION_UNKNOWN,
                      SBPL_MODIFIER);
  }

  pointer arg = scheme_arguments.next(sc, &args);
  if (!is_number(arg) || !is_integer(arg)) {
    fprintf(stderr, "\"send-signal\" received argument of invalid type, "
                    "requires integer argument.\n");

    return error_blob(sc, "Incorrect type of argument", LOCATION_UNKNOWN,
                      SBPL_MODIFIER);
  }

  long arg_val = ivalue(arg);

  return modifier_convert_to_blob(
      sc,
      (sbpl_modifier_t){
          // Making sure never to return references to local resources.
          .modifier_name = modifier_infos[modifier_idx_for_name("send-signal")]
                               .modifier_name,
          .argument_type = MODIFIER_ARGUMENT_INT,
          .argument.int_arg = arg_val});
}

pointer handle_report(scheme *sc, pointer args) {
  return handle_no_arguments_generic(sc, args, "report");
}

pointer handle_no_report(scheme *sc, pointer args) {
  return handle_no_arguments_generic(sc, args, "no-report");
}

pointer handle_no_sandbox(scheme *sc, pointer args) {
  return handle_no_arguments_generic(sc, args, "no-sandbox");
}

pointer handle_grant(scheme *sc, pointer args) {
  return handle_no_arguments_generic(sc, args, "grant");
}

pointer handle_rootless(scheme *sc, pointer args) {
  return handle_no_arguments_generic(sc, args, "rootless");
}

pointer handle_no_times(scheme *sc, pointer args) {
  return handle_no_arguments_generic(sc, args, "no-times");
}

pointer handle_default_sandbox(scheme *sc, pointer args) {
  size_t nargs = scheme_arguments.count(sc, args);

  if (nargs != 1) {
    fprintf(
        stderr,
        "\"default-sandbox\" received %zu arguments, but needs %d argument\n",
        nargs, 1);

    return error_blob(sc, "Incorrect number of args", LOCATION_UNKNOWN,
                      SBPL_MODIFIER);
  }

  pointer arg = scheme_arguments.next(sc, &args);
  if (!is_string(arg)) {
    fprintf(stderr, "\"default-sandbox\" received argument of invalid type, "
                    "requires string argument.\n");

    return error_blob(sc, "Incorrect type of argument", LOCATION_UNKNOWN,
                      SBPL_MODIFIER);
  }

  return modifier_convert_to_blob(
      sc, (sbpl_modifier_t){
              // Making sure never to return references to local resources.
              .modifier_name =
                  modifier_infos[modifier_idx_for_name("default-sandbox")]
                      .modifier_name,
              .argument_type = MODIFIER_ARGUMENT_STR,
              .argument.string_arg = string_value(arg)});
}

typedef pointer (*scheme_func)(scheme *sc, pointer);
static const scheme_func modifier_handlers[] = {NULL,
                                                handle_send_signal,
                                                handle_report,
                                                handle_no_report,
                                                handle_no_sandbox,
                                                handle_grant,
                                                handle_rootless,
                                                handle_no_times,
                                                handle_default_sandbox};

/**
 * Handler for the "with" statement that is used to declare modifiers.
 * Uses a dispatch table internally, to invoke the correct function
 * (which was already written previously)
 *
 * @param sc Current scheme context
 * @param args Arguments to scheme function
 * @return See the individual modifier handlers. Blob describing this specific
 * modifier.
 */
static pointer handle_modifier_statement(scheme *sc, pointer args) {
  size_t nargs = scheme_arguments.count(sc, args);
  assert(nargs >= 1);

  pointer first_arg = scheme_arguments.next(sc, &args);
  assert(is_string(first_arg));

  const char *modifier_name = string_value(first_arg);
  const size_t modifier_idx = modifier_idx_for_name(modifier_name);
  assert(modifier_idx > 0 && modifier_idx < modifiers_get_count());

  return modifier_handlers[modifier_idx](sc, args);
}

static void modifiers_register_handler(scheme *sc) {
  // Register modifier names with a string representation of them
  for (size_t i = 1; i < modifiers_get_count(); ++i) {
    scheme_support.register_object(
        sc, modifier_infos[i].modifier_name,
        mk_string(sc, modifier_infos[i].modifier_name));
  }

  // Register a single handler function for the "with" statement
  scheme_support.register_function(
      sc, (foreign_function_t){.symname = "with",
                               .foreign_function = handle_modifier_statement});
}

const sbpl_modifier_interface_t modifiers = {.register_handlers =
                                                 modifiers_register_handler};
