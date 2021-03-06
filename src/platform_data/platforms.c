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

#include "mojave/filters.h"
#include "mojave/operations.h"

#include "catalina/filters.h"
#include "catalina/operations.h"

#include <misc/os_support.h>

struct platform_info {
  enum target_platform identifier;
  const char *version_prefix;
  const char *release_name;
};

const struct platform_info supported_platforms[] = {
    {PLATFORM_SIERRA, "10.12", "sierra"},
    {PLATFORM_HIGH_SIERRA, "10.13", "high_sierra"},
    {PLATFORM_MOJAVE, "10.14", "mojave"},
    {PLATFORM_CATALINA, "10.15", "catalina"}};

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
  enum target_platform platform = PLATFORM_UNKNOWN;

  char *version = macos_version();
  if (strncmp("10.12", version, 5) == 0) {
    platform = PLATFORM_SIERRA;
  } else if (strncmp("10.13", version, 5) == 0) {
    platform = PLATFORM_HIGH_SIERRA;
  } else if (strncmp("10.14", version, 5) == 0) {
    platform = PLATFORM_MOJAVE;
  } else if (strncmp("10.15", version, 5) == 0) {
    platform = PLATFORM_CATALINA;
  }
  free(version);

  return platform;
}

enum target_platform platform_from_string(const char *platform_str) {
  for (size_t i = 0; i < n_supported_platforms; ++i) {
    if (strcmp(supported_platforms[i].release_name, platform_str) == 0) {
      return supported_platforms[i].identifier;
    }
  }

  return PLATFORM_UNKNOWN;
}

const char *version_prefix_for_platform(enum target_platform platform) {
  for (size_t i = 0; i < n_supported_platforms; ++i) {
    if (supported_platforms[i].identifier == platform) {
      return supported_platforms[i].version_prefix;
    }
  }

  return NULL;
}

op_data_provider operations_for_platform(enum target_platform platform) {
  switch (platform) {
  case PLATFORM_SIERRA:
    return sierra_ops;
  case PLATFORM_HIGH_SIERRA:
    return high_sierra_ops;
  case PLATFORM_MOJAVE:
    return mojave_ops;
  case PLATFORM_CATALINA:
    return catalina_ops;
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
  case PLATFORM_MOJAVE:
    return mojave_filters;
  case PLATFORM_CATALINA:
    return catalina_filters;
  default:
    assert(false && "Invalid platform specified.");
  }
}