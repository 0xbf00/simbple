//
// Created by Jakob Rieck on 11.02.18.
//
#ifndef SBPL_META_FILTERS_DEFINITION_H
#define SBPL_META_FILTERS_DEFINITION_H

#include "meta-filters.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct cell;
typedef struct cell *pointer;
typedef struct scheme scheme;

/**
 * Implement the "require-not" filter. Not meant to be called directly.
 * @param sc The current scheme context
 * @param args The arguments from the scheme runtime
 * @return A new value to be passed to other functions
 */
pointer require_not(scheme *sc, pointer args);

/**
 * Implement the "require-any" filter. Not meant to be called directly.
 * @param sc The current scheme context
 * @param args The arguments from the scheme runtime
 * @return A new value to be passed to other functions
 */
pointer require_any(scheme *sc, pointer args);

/**
 * Implement the "require-all" filter. Not meant to be called directly.
 * @param sc The current scheme context
 * @param args The arguments from the scheme runtime
 * @return A new value to be passed to other functions
 */
pointer require_all(scheme *sc, pointer args);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_META_FILTERS_DEFINITION_H