//
// Created by Jakob Rieck on 19.02.18.
//

#ifndef SBPL_REGEX_H
#define SBPL_REGEX_H

#include <scheme_interpreter/scheme.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Regex-quotes a string argument. Needed by the sandbox.
 * This function is meant to be bound to the "regex-quote" scheme symbol.
 *
 * @param sc The current scheme environment
 * @param args The arguments to this function
 * @return A new string argument that is quoted. Returns sc->NIL on failure.
 */
pointer regex_quote(scheme *sc, pointer args);

#ifdef __cplusplus
}
#endif

#endif // SBPL_REGEX_H
