//
// Created by Jakob Rieck on 20.02.18.
//

#include "string.h"
#include <assert.h>
#include <misc/misc.h>
#include <string.h>

#include <scheme_interpreter/scheme-private.h>

/**
 * Returns an array of pointers to the individual split parts. NULL denotes the
 * last entry.
 * @param to_split String to split up.
 * @param by Split at this string
 * @return Array of pointers denoting the individual components.
 */
static const char **split_string_internal(const char *to_split, const char *by,
                                          size_t *num_results) {
  const char *current = to_split;
  char *split_start;

  size_t n_results = 0;
  const char **results = NULL;

  do {
    split_start = strstr(current, by);

    size_t required_size = 0;

    if (!split_start) {
      // Copy the remaining last part to buffer.
      required_size = strlen(current) + 1;
    } else {
      required_size = split_start - current;
    }

    // Copy part of the string to buffer
    char *part_output = calloc(required_size + 1, 1);
    strncpy(part_output, current, required_size);

    results = realloc(results, sizeof(char *) * (n_results + 2));
    assert(results && "Unable to allocate memory.");

    results[n_results] = part_output;
    results[n_results + 1] = NULL;

    n_results++;

    current = split_start + strlen(by);
  } while (split_start != NULL);

  *num_results = n_results;

  return results;
}

pointer split_string(scheme *sc, pointer args) {
  const size_t n_args = scheme_arguments.count(sc, args);
  assert(n_args == 2);

  pointer first_arg = scheme_arguments.next(sc, &args);
  assert(is_string(first_arg));
  pointer second_arg = scheme_arguments.next(sc, &args);
  assert(is_string(second_arg));

  size_t num_results = 0;
  const char **splits = split_string_internal(
      string_value(first_arg), string_value(second_arg), &num_results);
  pointer output = sc->NIL;

  // Last entry should be NULL
  assert(splits[num_results] == NULL);

#if defined(ORDERING_LEGACY)
#warning "Using legacy ordering."
  for (size_t i = 0; *(splits + i) != NULL; ++i) {
    output = cons(sc, mk_string(sc, *(splits + i)), output);
    free(*(splits + i));
  }
#else
  for (size_t i = num_results; i != 0; --i) {
    output = cons(sc, mk_string(sc, splits[i - 1]), output);
    free(splits[i - 1]);
  }
#endif

  free(splits);

  return output;
}