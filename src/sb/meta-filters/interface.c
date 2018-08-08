//
// Created by Jakob Rieck on 11.02.18.
//

#include "definition.h"
#include "meta-filters.h"

#include <misc/misc.h>

/**
 * Registers the handlers for "require-all", "require-any" and "require-not"
 */
static void register_handlers(scheme *sc) {
  const foreign_function_t handlers[] = {
      {.symname = "require-all", .foreign_function = require_all},
      {.symname = "require-any", .foreign_function = require_any},
      {.symname = "require-not", .foreign_function = require_not}};

  scheme_support.register_functions(sc, handlers,
                                    sizeof(handlers) / sizeof(*handlers));
}

const sbpl_meta_filter_interface_t meta_filters = {.register_handlers =
                                                       register_handlers};