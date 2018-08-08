#include "data.h"
#include "mem.h"
#include "operations.h"
#include "types.h"

#include <assert.h>
#include <misc/misc.h>
#include <sb/error.h>
#include <string.h>

#include <scheme_interpreter/scheme-private.h>

static pointer handle_generic_operation(scheme *sc, pointer args) {
  size_t nargs = scheme_arguments.count(sc, args);

  if (nargs != 1) {
    return error_blob(sc, "Excess arguments for operation.", LOCATION_UNKNOWN,
                      SBPL_OPERATION);
  }

  pointer first_arg = scheme_arguments.next(sc, &args);
  assert(is_string(first_arg) && "Generic operation handler called without "
                                 "string. This should not happen.");

  const char *op_str = string_value(first_arg);

  return scheme_blob(sc, operation_create(op_str), SBPL_OPERATION);
}

static void register_handlers_for_system(scheme *sc,
                                         op_data_provider data_provider) {
  scheme_support.register_function(
      sc, (foreign_function_t){.symname = "operation-generic",
                               .foreign_function = handle_generic_operation});

  operations_install(data_provider);

  for (size_t i = 0; i < operations_get_count(); ++i) {
    // Redirect each operation to the generic operation handler
    const char *op_name = operation_name_for_idx(i);

    bool success =
        scheme_support.redirect_symbol(sc, op_name, "operation-generic");
    assert(success && "Failed to redirect operation.");
  }
}

const sbpl_operation_interface_t operations = {
    .register_handlers = register_handlers_for_system};