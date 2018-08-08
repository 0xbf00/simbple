#include "argument_handling.h"
#include <assert.h>

#include <scheme_interpreter/scheme-private.h>
#include <scheme_interpreter/scheme.h>

// Returns the number of arguments supplied to a
// given function.
size_t scheme_num_args(scheme *sc, pointer args) {
  size_t nargs = 0;

  while (args != sc->NIL) {
    nargs++;
    args = pair_cdr(args);
  }

  return nargs;
}

// Returns the next argument for a given argument list
// modifies the input parameter args, to allow the next
// call to fetch the next argument.
pointer scheme_next_arg(scheme *sc, pointer *args) {
  assert(args != NULL);

  if (*args == sc->NIL) {
    return sc->NIL;
  }

  pointer arg = pair_car(*args);
  *args = pair_cdr(*args);

  return arg;
}

const argument_handling_interface_t scheme_arguments = {
    .count = scheme_num_args, .next = scheme_next_arg};