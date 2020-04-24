//
// Created by Jakob Rieck on 13.02.18.
//

#include "actions.h"

#include <assert.h>
#include <misc/misc.h>
#include <scheme_interpreter/scheme-private.h>
#include <string.h>

#include "sb.h"
#include "sb/rule_base.h"

static void rule_add_filter(sbpl_action_t *rule, pointer filter) {
  assert(is_blob(filter));
  assert(blob_info(filter).tag == SBPL_FILTER ||
         blob_info(filter).tag == SBPL_META_FILTER);

  rule->filters =
      realloc(rule->filters, sizeof(pointer) * (rule->nfilters + 1));
  assert(rule->filters && "Unable to allocate memory.");

  rule->filters[rule->nfilters] = filter;
  rule->nfilters += 1;
}

static void rule_add_modifier(sbpl_action_t *rule, pointer modifier) {
  assert(is_blob(modifier));
  assert(blob_info(modifier).tag == SBPL_MODIFIER);

  rule->modifiers =
      realloc(rule->modifiers, sizeof(pointer) * (rule->nmodifiers + 1));
  assert(rule->modifiers && "Unable to allocate memory.");

  rule->modifiers[rule->nmodifiers] = modifier;
  rule->nmodifiers += 1;
}

static void rule_add_operation(sbpl_action_t *rule, pointer operation) {
  assert(is_blob(operation));
  blob_info_t bi = blob_info(operation);
  assert(bi.tag == SBPL_OPERATION);

  rule->operations =
      realloc(rule->operations, sizeof(pointer) * (rule->noperations + 1));
  assert(rule->operations && "Unable to allocate memory.");

  rule->operations[rule->noperations] = operation;
  rule->noperations += 1;
}

/**
 * Frees an action. Used internally by the Scheme runtime during garbage
 * collection.
 * @param action The action to free
 */
void action_free(sbpl_action_t *action) {
  free(action->operations);
  free(action->filters);
  free(action->modifiers);

  free(action);
}

static pointer sbpl_create_rule(scheme *sc, pointer args) {
  const size_t nargs = scheme_arguments.count(sc, args);
  assert(nargs > 1);

  // The first argument is the action to take (either allow or deny)
  pointer first_arg = scheme_arguments.next(sc, &args);
  assert(is_string(first_arg));

  sbpl_action_t *rule = calloc(1, sizeof(sbpl_action_t));

  assert(rule && "Unable to allocate memory for rule.");

  const char *action = string_value(first_arg);

  assert((!strcmp(action, "allow") || !strcmp(action, "deny")) &&
         "Invalid action supplied.");

  if (!strcmp(action, "allow")) {
    rule->type = ALLOW;
  } else { // if (!strcmp(action, "deny")) {
    rule->type = DENY;
  }

  for (size_t i = 1; i < nargs; ++i) {
    pointer next_arg = scheme_arguments.next(sc, &args);

    assert(next_arg != sc->NIL);
    // macOS Catalina uses the 'no-op symbol to neuter operations that
    // are still part of some sandbox profiles but that have no effect
    // anymore. We can simply discard / ignore these rules.
    if ( (i == 1) /* should be the first argument (the operation) */
      && is_symbol(next_arg) /* needs to be the "no-op" symbol */
      && (0 == strcmp("no-op", symname(next_arg))))
    {
      free(rule);
      return sc->NIL;
    }

    assert(is_blob(next_arg));

    blob_info_t bi = blob_info(next_arg);
    if (bi.tag == SBPL_MODIFIER) {
      rule_add_modifier(rule, next_arg);
    } else if (bi.tag == SBPL_FILTER || bi.tag == SBPL_META_FILTER) {
      rule_add_filter(rule, next_arg);
    } else if (bi.tag == SBPL_OPERATION) {
      rule_add_operation(rule, next_arg);
    } else if (bi.tag == SBPL_ERROR) {
      printf("[Error]: %s\n", ((sbpl_error_t *)bi.blob)->description);
      assert(false && "Error occurred.");
    } else {
      assert(false && "Invalid argument supplied.");
    }
  }

  rule_base_add_rule(rule);

  return scheme_blob(sc, rule, SBPL_ACTION);
}

static void register_handlers(scheme *sc) {
  // Register one generic function...
  scheme_support.register_function(
      sc, (foreign_function_t){"sbpl-create-rule", sbpl_create_rule});

  // which handles both allow and deny rules.
  scheme_support.redirect_function(sc, "allow", "sbpl-create-rule",
                                   "\"allow\"");
  scheme_support.redirect_function(sc, "deny", "sbpl-create-rule", "\"deny\"");
}

const sbpl_action_interface_t actions = {.register_handlers =
                                             register_handlers};