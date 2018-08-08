//
// Created by Jakob Rieck on 10.02.18.
//

#include "data.h"

#include <assert.h>
#include <string.h>

struct operations {
  char **names;
  operation_info_t *infos;
  size_t count;
};

static struct operations *sb_ops = NULL;

/**
 * Install the specified operations structure to be used by the interpreter.
 * This function does not take ownership of the underlying data. The caller
 * is expected to retain the data indefinitely. What also works is if the
 * data in question is stored statically in program data.
 */
void operations_install(op_data_provider provider) {
  assert(sb_ops == NULL && "sb_ops structure already initialised.");

  sb_ops = calloc(sizeof(struct operations), 1);
  assert(sb_ops != NULL && "Failed to allocate memory.");

  sb_ops->count = provider(&sb_ops->names, &sb_ops->infos);
}

/**
 * Returns the number of sandbox operations currently known.
 */
const size_t operations_get_count() { return sb_ops->count; }

/**
 * Return the name of the ith operation
 *
 * @param operation_idx The index of the operation.
 * @return The name of the operation
 */
const char *operation_name_for_idx(const size_t operation_idx) {
  assert(sb_ops != NULL && "sb_ops structure not yet initialised.");

  if (operation_idx >= sb_ops->count)
    return NULL;

  return sb_ops->names[operation_idx];
}

/**
 * Return the operation_info_t structure for a given (valid) index
 *
 * @param operation_idx The index of the operation.
 * @return A pointer to the desired operation_info_t structure or NULL, if
 * invalid.
 */
const operation_info_t *const
operation_info_for_idx(const size_t operation_idx) {
  assert(sb_ops != NULL && "sb_ops structure not yet initialised.");

  if (operation_idx >= sb_ops->count)
    return NULL;

  return &sb_ops->infos[operation_idx];
}

/**
 * Return the operation_info_t structure for a given operation name.
 * @param operation_name The name of the operation
 * @return Pointer to the operation_info_t entry or NULL, if not found
 */
const operation_info_t *const
operation_info_for_name(const char *operation_name) {
  const size_t index = operation_idx_for_name(operation_name);

  return (index == OPERATION_NOT_FOUND) ? NULL : &sb_ops->infos[index];
}

/**
 * Return the operation idx (often used internally as an identifier) for a given
 * operation_info_t structure.
 * @param operation_info The structure describing an operation.
 * @return The operation index or OPERATION_NOT_FOUND if not found
 */
const size_t
operation_idx_for_operation_info(const operation_info_t *const operation_info) {
  assert(sb_ops != NULL && "sb_ops structure not yet initialised.");

  size_t diff = operation_info - &sb_ops->infos[0];

  if (diff > sb_ops->count)
    return OPERATION_NOT_FOUND;

  return diff;
}

/**
 * Return the operation idx (often used internally as an identifier) for a given
 * operation name.
 * @param operation_name The name of the operation
 * @return The operation index or OPERATION_NOT_FOUND if not found
 */
const size_t operation_idx_for_name(const char *operation_name) {
  assert(sb_ops != NULL && "sb_ops structure not yet initialised.");

  for (size_t i = 0; i < sb_ops->count; ++i) {
    if (strcmp(sb_ops->names[i], operation_name) == 0) {
      return i;
    }
  }

  return OPERATION_NOT_FOUND;
}
