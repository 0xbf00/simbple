#include "platforms.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Platform-specific data includes
#include "sierra/filters.h"
#include "sierra/operations.h"

#include "high_sierra/filters.h"
#include "high_sierra/operations.h"

#include <misc/os_support.h>

struct platform_info {
  enum target_platform identifier;
  const char *version_prefix;
  const char *release_name;
};

const struct platform_info supported_platforms[] = {
    {PLATFORM_SIERRA, "10.12", "sierra"},
    {PLATFORM_HIGH_SIERRA, "10.13", "high_sierra"}};

const size_t n_supported_platforms =
    sizeof(supported_platforms) / sizeof(*supported_platforms);

const char *list_target_platforms() {
  enum target_platform default_platform = platform_get_default();

  char *target_platforms = NULL;

  for (size_t i = 0; i < n_supported_platforms; ++i) {
    if (target_platforms) {
      char *old_value = target_platforms;

      int success = asprintf(
          &target_platforms, "%s, %s%s", old_value,
          supported_platforms[i].release_name,
          (supported_platforms[i].identifier == default_platform) ? " (default)"
                                                                  : "");
      assert(success != -1 && "Unable to asprintf()");

      free(old_value);
    } else {
      int success = asprintf(
          &target_platforms, "%s%s", supported_platforms[i].release_name,
          (supported_platforms[i].identifier == default_platform) ? " (default)"
                                                                  : "");
      assert(success != -1 && "Unable to asprintf()");
    }
  }

  return target_platforms;
}

enum target_platform platform_get_default() {
  const char *version = macos_version();
  if (strncmp("10.12", version, 5) == 0) {
    return PLATFORM_SIERRA;
  } else if (strncmp("10.13", version, 5) == 0) {
    return PLATFORM_HIGH_SIERRA;
  } else {
    return PLATFORM_UNKNOWN;
  }
}

enum target_platform platform_from_string(const char *platform_str) {
  for (size_t i = 0; i < n_supported_platforms; ++i) {
    if (strcmp(supported_platforms[i].release_name, platform_str) == 0) {
      return supported_platforms[i].identifier;
    }
  }

  return PLATFORM_UNKNOWN;
}

op_data_provider operations_for_platform(enum target_platform platform) {
  switch (platform) {
  case PLATFORM_SIERRA:
    return sierra_ops;
  case PLATFORM_HIGH_SIERRA:
    return high_sierra_ops;
  default:
    assert(false && "Invalid platform specified.");
  }
}

filter_data_provider filters_for_platform(enum target_platform platform) {
  switch (platform) {
  case PLATFORM_SIERRA:
    return sierra_filters;
  case PLATFORM_HIGH_SIERRA:
    return high_sierra_filters;
  default:
    assert(false && "Invalid platform specified.");
  }
}