#ifndef SBPL_MODIFIERS_H
#define SBPL_MODIFIERS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct cell;
typedef struct scheme scheme;
typedef struct cell *pointer;

// -------------------------------------------
// Type definition for interface declaration
// -------------------------------------------
typedef struct {
  void (*register_handlers)(scheme *);
} sbpl_modifier_interface_t;

extern const sbpl_modifier_interface_t modifiers;

// -------------------------------------------
// modifier_info structure declaration. Should
// translate over to libsandbox easily
// -------------------------------------------
#define ACTION_ALL ((uint32_t)~0)
#define ACTION_ALLOW ((uint32_t)2)
#define ACTION_DENY ((uint32_t)1)
#define ACTION_NONE ((uint32_t)0)

// Maps directly to _modifier_info symbol in libsandbox!
#pragma pack(1)
typedef struct {
  const char *modifier_name;
  uint32_t applies_to_action; // What action (allow, deny, none, all) this
                              // modifier applies to.
  uint8_t action_mask;        // the meaning of action_mask...
  uint8_t action_flag;        // and action_flag is not entirely clear
} modifier_info_t;
#pragma pack()

extern const modifier_info_t modifier_infos[];

/**
 * Return the number of currently known modifiers.
 * @return The number of currently known modifiers.
 */
size_t modifiers_get_count();

// -------------------------------------------
// "emulator" modifier return type.
// Contains information needed for matching.
// -------------------------------------------

// Modelling all argument types that occur in the real
// sandbox implementation (as of 10.12.6)
enum modifier_argument_t {
  MODIFIER_ARGUMENT_NONE,
  MODIFIER_ARGUMENT_INT,
  MODIFIER_ARGUMENT_STR
};

/**
 * The return type that all modifier handlers return. It retains the information
 * about which modifier was used and which argument (if any) was used and can be
 * used for matching later on.
 */
#pragma pack(1)
typedef struct {
  const char *modifier_name;
  enum modifier_argument_t argument_type;

  union {
    const char *string_arg;
    long int_arg;
  } argument;
} sbpl_modifier_t;
#pragma pack()

/**
 * Frees a sbpl_modifier_t object. Exported to be used by the scheme runtime.
 * @param modifier The modifier to free. All resources, as well as the modifier
 * are freed.
 */
void modifier_free(sbpl_modifier_t *modifier);

#ifdef __cplusplus
}
#endif

#endif // SBPL_MODIFIERS_H