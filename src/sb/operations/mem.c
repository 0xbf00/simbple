//
// Created by Jakob Rieck on 16.02.18.
//
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "mem.h"
#include <sb/blob_handling.h>

/**
 * Create a new operation object.
 *
 * @param operation_name The name of the operation. This function
 * creates a copy of the string for internal purposes. Ensures that
 * the operation name refers to a valid operation.
 * @return operation object carrying associated information.
 */
sbpl_operation_t *operation_create(const char *operation_name) {
  assert(operation_idx_for_name(operation_name) != OPERATION_NOT_FOUND);

  sbpl_operation_t *mem = calloc(1, sizeof(sbpl_operation_t));
  assert(mem && "Unable to get memory for operation.");

  mem->operation_name = strdup(operation_name);

  return mem;
}

/**
 * Frees an operation. Used by the Scheme runtime
 */
void operation_free(sbpl_operation_t *operation) {
  free(operation->operation_name);
  free(operation);
}
