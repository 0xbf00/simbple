/**
 * Header file containing functionality to interact with
 * SBPL operations. For this project, a custom interface
 * was written to interact in a simpler way with this functionality,
 * as compared to Apple's bytecode used internally.
 */
#ifndef SBPL_OPERATIONS_H
#define SBPL_OPERATIONS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scheme scheme;

typedef struct {
  // Although we don't have a real handler for the operations
  // functionality, we still keep this name for consistency with
  // the modifiers and filters functionality.
  void (*register_handlers)(scheme *, op_data_provider provider);
} sbpl_operation_interface_t;

extern const sbpl_operation_interface_t operations;

#ifdef __cplusplus
}
#endif

#endif