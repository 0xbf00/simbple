//
// Created by Jakob Rieck on 10.02.18.
//
#ifndef SBPL_OPERATIONS_DATA_H
#define SBPL_OPERATIONS_DATA_H

#include "types.h"
#include <stdbool.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Function to install / register a new set of operations
 * with the current runtime.
 */
void operations_install(op_data_provider provider);

/**
 * Returns the number of sandbox operations currently known.
 */
const size_t operations_get_count();

/**
 * Lookup the name of the ith operation.
 * Returns NULL in case the operation could not be found (the index
 * is invalid)
 */
const char *operation_name_for_idx(const size_t operation_idx);

/**
 *	Lookup operation_info_t structures by index and name.
 *	These routines return appropriate error values if the operation_idx
 *	is out of bounds (OPERATION_NOT_FOUND) or if the operation_name is
 *	not known (NULL).
 */
const operation_info_t *const
operation_info_for_idx(const size_t operation_idx);
const operation_info_t *const
operation_info_for_name(const char *operation_name);

/**
 * Lookup the operation index (often used internally as an identifier) for a
 * given for a given operation_info structure or an operation name. Returns the
 * operation index or OPERATION_NOT_FOUND if no corresponding internal entry
 * could be found.
 */
const size_t
operation_idx_for_operation_info(const operation_info_t *const operation_info);
const size_t operation_idx_for_name(const char *operation_name);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_OPERATIONS_HELPERS_H