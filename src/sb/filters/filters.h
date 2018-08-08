//
// Created by Jakob Rieck on 06.02.18.
//
#ifndef SBPL_FILTERS_H
#define SBPL_FILTERS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scheme scheme;

// Public interface
typedef struct {
  void (*register_handlers)(scheme *, filter_data_provider data_provider);
} sbpl_filter_interface_t;

extern const sbpl_filter_interface_t filters;

#ifdef __cplusplus
}
#endif

#endif // SBPL_FILTERS_H