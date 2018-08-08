/**
 * Copyright (C) Jakob Rieck, 2018
 *
 * The purpose of this source file is to encapsulate all the required
 * memory-related functions that are used to create, modify and copy the output
 * structures that are emitted from the operation handlers.
 */
#ifndef SBPL_OPERATIONS_MEM_H
#define SBPL_OPERATIONS_MEM_H

#include "types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Create a new operation object.
 *
 * @param operation_name The name of the operation. This function
 * creates a copy of the string for internal purposes.
 * @return operation object carrying associated information.
 */
sbpl_operation_t *operation_create(const char *operation_name);

/**
 * Frees an operation. Used by the Scheme runtime
 */
void operation_free(sbpl_operation_t *operation);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_OPERATIONS_MEM_H