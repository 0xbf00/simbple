//
// Created by Jakob Rieck on 19.02.18.
//

#include "regex.h"
#include <misc/argument_handling.h>
#include <scheme_interpreter/scheme-private.h>
#include <stdlib.h>
#include <string.h>

static char *regex_quote_internal(const char *input) {
  const size_t in_len = strlen(input);

  // These are the characters the sandbox escapes / quotes.
  const char *to_quote = "^.[$()|*+?{\\";

  size_t index = strcspn(input, to_quote);

  if (input[index] == '\0') {
    // Nothing to quote found, simply return the input string.
    return input;
  }

  size_t count = 1;
  while (input[index] != '\0') {
    // Index calculation: Adds one because we start at the current index + 1.
    index += strcspn(input + index + 1, to_quote) + 1;
    if (input[index] != '\0')
      count++;
  }

  char *out = calloc(strlen(input) + count + 1, 1);

  size_t out_index = 0;
  for (size_t in_index = 0; in_index < strlen(input); ++in_index) {
    int in_char = input[in_index];

    if (memchr(to_quote, in_char, strlen(to_quote) + 1)) {
      out[out_index] = '\\';
      out_index++;
    }

    out[out_index] = input[in_index];
    out_index++;
  }

  return out;
}

/**
 * Regex-quotes a string argument. Needed by the sandbox.
 * This function is meant to be bound to the "regex-quote" scheme symbol.
 *
 * @param sc The current scheme environment
 * @param args The arguments to this function
 * @return A new string argument that is quoted. Returns sc->NIL on failure.
 */
pointer regex_quote(scheme *sc, pointer args) {
  const size_t n_args = scheme_arguments.count(sc, args);
  if (n_args != 1) {
    return sc->NIL;
  }

  pointer first_arg = scheme_arguments.next(sc, &args);
  if (!is_string(first_arg)) {
    return sc->NIL;
  }

  const char *in = string_value(first_arg);
  const char *out = regex_quote_internal(in);

  return mk_counted_string(sc, out, strlen(out));
}