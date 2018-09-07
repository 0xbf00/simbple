//
// Created by Jakob Rieck on 04.02.18.
//

#ifndef MISC_SCHEME_SUPPORT_H
#define MISC_SCHEME_SUPPORT_H

#include <scheme_interpreter/scheme.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
  const char *symname;

  pointer (*foreign_function)(scheme *, pointer);
} foreign_function_t;

typedef struct {
  void (*register_function)(scheme *, const foreign_function_t);

  void (*register_functions)(scheme *sc, const foreign_function_t *const,
                             const size_t);

  void (*register_object)(scheme *sc, const char *, pointer obj);

  int (*evaluate_string)(scheme *, const char *);

  bool (*redirect_function)(scheme *, const char *, const char *, const char *);

  bool (*redirect_symbol)(scheme *, const char *, const char *);

  void (*load_file)(scheme *, const char *);

  void (*load_string)(scheme *, const char *);
} scheme_support_interface_t;

/**
 * Registers a scheme object `obj` to a symbol name with the scheme runtime.
 *
 * @param sc The current scheme context
 * @param symname The desired symbol name
 * @param obj The scheme object to bind to the symbol name
 */
void register_object(scheme *sc, const char *symname, pointer obj);

/**
 * Binds a given function to a symbol name.
 * Convenience function, saves the caller from
 * calling two separate functions.
 *
 * @param sc The current scheme context
 * @param function Structure containing the desired symbol name and the
 * corresponding foreign function.
 * Note that this function has to follow TinySCHEME's specifications,
 * meaning its result type and parameter type are fixed.
 */
void register_function(scheme *sc, const foreign_function_t function);

/**
 * Binds a number of functions to a number of symbol names. See also
 * register_function!
 * @param sc The current scheme context
 * @param functions An array of foreign_function objects.
 * @param count The number of foreign_function objects to register
 */
void register_functions(scheme *sc, const foreign_function_t *const functions,
                        size_t count);

/**
 * Redirects a function to another function. The function to which to redirect
 * that to be known to the Scheme runtime already.
 *
 * @param sc The current scheme context
 * @param from The function name which to redirect
 * @param to The function name to redirect to. Note that this function should
 * already be known to the runtime.
 * @param argument A possible argument that is given as a first argument to `to`
 * and that can for instance be used to retain the information which symbol was
 * redirected. This argument, if != NULL, is prepended to the argument list.
 * @return True, iff the redirection was put in place successfully, else false
 */
bool redirect_function(scheme *sc, const char *from, const char *to,
                       const char *argument);

/**
 * Redirects a symbol to a function. Useful when you want to map multiple
 * symbols to the same function. Automatically calls the redirector function
 * with the name of the symbol which was redirected.
 * @param sc The current scheme context
 * @param from_symbol The symbol name which to redirect.
 * @param to_function The function name to redirect to.
 * @return True, iff the redirection was put in place successfully, else false.
 */
bool redirect_symbol(scheme *sc, const char *from_symbol,
                     const char *to_function);

/**
 * Evaluate a SCHEME string and return a result code indicating whether
 * the execution was successful (0) or not (!0).
 * @param sc The current scheme context
 * @param str The string to evaluate
 * @return Either 0 indicating success or an error code
 */
int evaluate_string(scheme *sc, const char *str);

/**
 * Checks whether the supplied argument is a scheme bool
 * @param sc The current scheme context.
 * @param arg The argument to check
 * @return True, if the argument is a boolean value
 */
bool is_bool(scheme *sc, pointer arg);

/**
 * Gets the value of a boolean argument.
 * @param sc The current scheme context.
 * @param arg The argument for which to get the boolean value
 * @return The boolean value of the argument
 */
bool bool_value(scheme *sc, pointer arg);

/**
 * Loads a file using the Scheme interpreter.
 * @param sc The current scheme context.
 * @param filename The file to load.
 */
void load_file(scheme *sc, const char *filename);

/**
 * Load a scheme command (or multiple commands)
 * using the Scheme interpreter.
 * @param sc The current scheme context.
 * @param scm The scheme commands to execute / load.
 */
void load_string(scheme *sc, const char *scm);

void line_identifier(scheme *sc);

extern const scheme_support_interface_t scheme_support;

#if defined(__cplusplus)
}
#endif

#endif // MISC_SCHEME_SUPPORT_H
