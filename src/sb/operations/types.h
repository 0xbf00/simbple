//
// Created by Jakob Rieck on 10.02.18.
//
#ifndef SBPL_OPERATIONS_TYPES_H
#define SBPL_OPERATIONS_TYPES_H

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define OPERATION_NOT_FOUND (~0)

enum operation_type { TERMINAL_NODE = 0, NONTERMINAL_NODE = 1 };

#pragma pack(1)
typedef struct {
  enum operation_type node_type;
  uint32_t fallback_op; // index of the fallback operation.
  // May not be the same one as this!
  uint32_t action; // So far, the exact purpose of this field is unclear.
  const char **allowed_filters;
  const char **allowed_modifiers;
} operation_info_t;
#pragma pack()

typedef const size_t (*op_data_provider)(const char ***op_names,
                                         const operation_info_t **op_infos);

// A single operation has no arguments or other state
// that would need to be preserved. As such, it is possible and
// useful to simply use a string to identify the operation, which
// saves us from worrying about memory management for the most part.
typedef struct {
  const char *operation_name;
} sbpl_operation_t;

#if defined(__cplusplus)
}
#endif

#endif // SBPL_OPERATIONS_TYPES_H