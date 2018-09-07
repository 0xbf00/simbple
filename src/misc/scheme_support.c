//
// Created by Jakob Rieck on 04.02.18.
//

#include "scheme_support.h"
#include "misc.h"
#include <assert.h>
#include <ctype.h>
#include <memory.h>
#include <scheme_interpreter/scheme-private.h>
#include <stdlib.h>

void register_object(scheme *sc, const char *symname, pointer obj) {
  pointer sym = mk_symbol(sc, symname);
  scheme_define(sc, sc->global_env, sym, obj);
}

void register_function(scheme *sc, const foreign_function_t foreign_function) {
  register_object(sc, foreign_function.symname,
                  mk_foreign_func(sc, foreign_function.foreign_function));
}

void register_functions(scheme *sc,
                        const foreign_function_t *const foreign_functions,
                        const size_t count) {
  for (size_t i = 0; i < count; ++i) {
    register_function(sc, foreign_functions[i]);
  }
}

int string_evaluate(scheme *sc, const char *str) {
  scheme_load_string(sc, str);
  return sc->retcode;
}

bool redirect_function(scheme *sc, const char *from, const char *to,
                       const char *argument) {
  char *scheme_cmd;

  if (argument) {
    asprintf(&scheme_cmd,
             /*
              * Scheme definition that redirects a given filter name to the
              * generic filter-generic handler (see also: High Level Overview)
              */
             "(define (%s . args) (apply %s (cons %s args)))", from, to,
             argument);
  } else {
    asprintf(&scheme_cmd,
             /*
              * For meaning, see above.
              */
             "(define (%s . args) (apply %s args))", from, to);
  }

  if (!scheme_cmd) {
    return false;
  } else {
    int res = string_evaluate(sc, scheme_cmd);

    free(scheme_cmd);
    return (res == 0);
  }
}

bool redirect_symbol(scheme *sc, const char *from_symbol,
                     const char *to_function) {
  char *scheme_cmd;

  asprintf(&scheme_cmd, "(define %s (%s \"%s\"))", from_symbol, to_function,
           from_symbol);

  if (!scheme_cmd)
    return false;

  int res = string_evaluate(sc, scheme_cmd);
  free(scheme_cmd);

  return (res == 0);
}

bool is_bool(scheme *sc, pointer arg) { return arg == sc->T || arg == sc->F; }

bool bool_value(scheme *sc, pointer arg) {
  assert(is_bool(sc, arg));

  return arg == sc->T;
}

void load_file(scheme *sc, const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    char *err_string = NULL;
    asprintf(&err_string, "Failed to open file %s: ", filename);
    perror(err_string);
    free(err_string);
    return;
  }

  scheme_load_file(sc, f);
  fclose(f);
}

void load_string(scheme *sc, const char *scm) {
  scheme_load_string(sc, scm);
}

const scheme_support_interface_t scheme_support = {
    .register_function = register_function,
    .register_functions = register_functions,
    .register_object = register_object,
    .evaluate_string = string_evaluate,
    .redirect_function = redirect_function,
    .redirect_symbol = redirect_symbol,
    .load_file = load_file,
    .load_string = load_string};
