//
// Created by Jakob Rieck on 13.02.18.
//

#ifndef SBPL_ACTIONS_H
#define SBPL_ACTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

struct cell;
typedef struct cell *pointer;
typedef struct scheme scheme;

enum action_type { ALLOW, DENY };

// The action type encapsulates every other
// piece of information relevant to the current
// rule. It essentially stores all the rule's contents.
#pragma pack(1)
typedef struct sbpl_action {
  enum action_type type;
  size_t noperations;
  pointer *operations;
  size_t nfilters;
  pointer *filters;
  size_t nmodifiers;
  pointer *modifiers;
} sbpl_action_t;
#pragma pack()

typedef struct {
  void (*register_handlers)(scheme *);
} sbpl_action_interface_t;

extern const sbpl_action_interface_t actions;

/**
 * Frees an action. Used internally by the Scheme runtime during garbage
 * collection.
 * @param action The action to free
 */
void action_free(sbpl_action_t *action);

#ifdef __cplusplus
}
#endif

#endif // SBPL_ACTIONS_H