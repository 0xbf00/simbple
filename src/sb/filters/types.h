//
// Created by Jakob Rieck on 07.02.18.
//
#ifndef SBPL_FILTERS_TYPES_H
#define SBPL_FILTERS_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define FILTER_NO_REQUISITE 0

// Used by the various helper function to denote that a certain filter entry
// could not be located
#define FILTER_NOT_FOUND (0)

/**
 * The sandbox supports a number of filter arguments that are
 * specific as an abstract name. In order to map these to their
 * underlying (integer) representation, the sandbox has to know
 * about them. Because we try to emulate the sandbox compilation,
 * we also have to care about them.
 *
 * For compatability with the libsandbox version, the aligned directive
 * is used.
 */
typedef struct {
  const char *name;
  uint16_t value;
} named_argument;

enum filter_argument_type {
  SB_VALUE_TYPE_BOOL = 0x1,
  SB_VALUE_TYPE_BITFIELD = 0x2,
  SB_VALUE_TYPE_INTEGER = 0x3,
  SB_VALUE_TYPE_STRING = 0x4,
  SB_VALUE_TYPE_PATTERN_LITERAL = 0x5,
  SB_VALUE_TYPE_PATTERN_PREFIX = 0x6,
  SB_VALUE_TYPE_PATTERN_SUBPATH = 0x7,
  SB_VALUE_TYPE_PATTERN_REGEX = 0x8,
  SB_VALUE_TYPE_REGEX = 0x9,
  SB_VALUE_TYPE_NETWORK = 0xA,
};

/**
 * Each filter is described by a filter_info entry that consists of the
 * following data items. The structure below is identical to the structure used
 * in the binary.
 */
#pragma pack(1)
typedef struct {
  const char *name;
  const char *category;
  /**
   * Type of arguments accepted by the filter.
   * the type stores values of the enum filter_argument_type
   * type. Whether a certain filter accepts a certain argument
   * type is decided by the _filter_accepts_type functions in
   * platform_data/example/filters.h
   *
   * Changes:
   * - Mojave: size changed to 8-bits from 32-bits.
   */
  uint8_t argument_type;
  /**
   * Shortcut to compute whether two filters are mutually exclusive.
   * Require-all metafilters where >= two containing filters are mutually
   * exclusive are removed during compilation
   *
   * Changes:
   * - Mojave: introduced
   */
  uint8_t is_contrary;
  /**
   * Initial cost factor of the filter. The actual usage of this field
   * is not clear and has not been reverse engineered. It is possible
   * that filters are sorted in increasing cost factor order as to minimise
   * the default access time.
   *
   * Changes:
   * - Mojave: introduced
   */
  uint16_t cost_factor;

  /**
   * Index of another filter, if this filter has a prerequisite, otherwise 0
   * (FILTER_NO_REQUISITE)
   */
  uint32_t prerequisite;

  /**
   * Pointer to array of `named_argument` structures. The list ends with an
   * empty argument structure where name and value are 0.
   */
  const named_argument *named_arguments;
} filter_info_t;
#pragma pack()

// -------------------------------------------
// "emulator" filter return type.
// Contains information needed for matching.
// -------------------------------------------

enum argument_type {
  // We make sure NOT to use the 0 value, because that allows us
  // to quickly check whether we forgot to specify the argument's type!
  PRIMITIVE = 1,
  BOOLEAN,
  STRING
};

/**
 * Modelling the arguments for a filter.
 *
 * A filter's type (sbpl_filter_t.arg_type) decides how to evaluate the
 * argument, either as a string like object (in case of patterns, regex, ...) or
 * as an integer like object.
 */
#pragma pack(1)
typedef struct {
  enum argument_type type;
  const char *alias;
  union {
    char *as_str;
    uint64_t as_int;
  } argument;
} sbpl_filter_argument_t;
#pragma pack()

/**
 * The return type that the generic filter handler returns. It retains the
 * information about which filter was originally used and which argument (if
 * any) was used and can be used for matching later on.
 */
typedef struct {
  const char *filter_name;
  size_t n_arguments;
  sbpl_filter_argument_t *arguments;
} sbpl_filter_t;

typedef bool (*filter_accept_type_func)(const filter_info_t *const filter,
                                        enum filter_argument_type type);

typedef const size_t (*filter_data_provider)(
    const filter_info_t **filter_infos, filter_accept_type_func *acceptor_func);

#if defined(__cplusplus)
}
#endif

#endif // SBPL_FILTERS_TYPES_H