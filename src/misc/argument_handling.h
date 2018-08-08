#ifndef MISC_ARGUMENT_HANDLING_H
#define MISC_ARGUMENT_HANDLING_H

#include <stdint.h>
#include <stdlib.h>

#include <scheme_interpreter/scheme.h>

#if defined(__cplusplus)
extern "C" {
#endif

// Together with the extern definition at the bottom of this file,
// this constitutes one way to implement "namespaces" in C.
typedef struct {
  size_t (*count)(scheme *sc, pointer);

  pointer (*next)(scheme *sc, pointer *);
} argument_handling_interface_t;

/**
 * Returns the number of arguments supplied to a function.
 *
 * No internal state is changed.
 * @param sc The current scheme context.
 * @param args Pointer to a linked list containing the arguments supplied to a
 * function
 * @return Number of arguments supplied to a function
 */
size_t scheme_num_args(scheme *sc, pointer args);

/**
 * Returns the next argument from the argument list args.
 *
 * Writes back to the args argument, in order to allow
 * subsequent calls to retrieve all arguments.
 *
 * @param sc The current scheme context.
 * @param args Pointer to pointer to a linked list containing the arguments
 * supplied to a function.
 * @return Pointer to the next argument. The args pointer is updated to allow
 * another call to this function.
 */
pointer scheme_next_arg(scheme *sc, pointer *args);

extern const argument_handling_interface_t scheme_arguments;

#if defined(__cplusplus)
}
#endif

#endif // MISC_ARGUMENT_HANDLING_H