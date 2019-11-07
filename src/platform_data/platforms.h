#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <sb/filters/types.h>
#include <sb/operations/types.h>

#include <stdlib.h>

enum target_platform {
  PLATFORM_UNKNOWN,
  PLATFORM_SIERRA,
  PLATFORM_HIGH_SIERRA,
  PLATFORM_MOJAVE
};

const char *list_target_platforms();

enum target_platform platform_from_string(const char *platform_str);
enum target_platform platform_get_default();

const char *version_prefix_for_platform(enum target_platform platform);
op_data_provider operations_for_platform(enum target_platform platform);
filter_data_provider filters_for_platform(enum target_platform platform);

#endif