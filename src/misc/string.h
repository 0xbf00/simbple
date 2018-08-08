//
// Created by Jakob Rieck on 20.02.18.
//

#ifndef SBPL_STRING_H
#define SBPL_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <scheme_interpreter/scheme.h>

/**
 * Splits an input string on another string, returning a list of results.
 * @param sc The current scheme context
 * @param args Arguments. Two arguments are required, both should be strings.
 * @return A list of strings
 */
pointer split_string(scheme *sc, pointer args);

#ifdef __cplusplus
}
#endif

#endif // SBPL_STRING_H
