#include "filters.h"

static const named_argument local_args[] = {
    {"ip", 3},   {"ip4", 1},  {"ip6", 2},  {"tcp", 7},   {"tcp4", 5},
    {"tcp6", 6}, {"udp", 11}, {"udp4", 9}, {"udp6", 10}, {NULL, 0}};
static const named_argument remote_args[] = {
    {"ip", 3},   {"ip4", 1},  {"ip6", 2},  {"tcp", 7},   {"tcp4", 5},
    {"tcp6", 6}, {"udp", 11}, {"udp4", 9}, {"udp6", 10}, {NULL, 0}};
static const named_argument socket_domain_args[] = {
    {"AF_UNSPEC", 0},       {"AF_UNIX", 1},
    {"AF_LOCAL", 1},        {"AF_INET", 2},
    {"AF_IMPLINK", 3},      {"AF_PUP", 4},
    {"AF_CHAOS", 5},        {"AF_NS", 6},
    {"AF_ISO", 7},          {"AF_OSI", 7},
    {"AF_ECMA", 8},         {"AF_DATAKIT", 9},
    {"AF_CCITT", 10},       {"AF_SNA", 11},
    {"AF_DECnet", 12},      {"AF_DLI", 13},
    {"AF_LAT", 14},         {"AF_HYLINK", 15},
    {"AF_APPLETALK", 16},   {"AF_ROUTE", 17},
    {"AF_LINK", 18},        {"pseudo_AF_XTP", 19},
    {"AF_COIP", 20},        {"AF_CNT", 21},
    {"pseudo_AF_RTIP", 22}, {"AF_IPX", 23},
    {"AF_SIP", 24},         {"pseudo_AF_PIP", 25},
    {"AF_NDRV", 27},        {"AF_ISDN", 28},
    {"AF_E164", 28},        {"pseudo_AF_KEY", 29},
    {"AF_INET6", 30},       {"AF_NATM", 31},
    {"AF_SYSTEM", 32},      {"AF_NETBIOS", 33},
    {"AF_PPP", 34},         {"pseudo_AF_HDRCMPLT", 35},
    {"AF_IEEE80211", 37},   {"AF_UTUN", 38},
    {"AF_MAX", 40},         {NULL, 0}};
static const named_argument socket_type_args[] = {
    {"SOCK_STREAM", 1}, {"SOCK_DGRAM", 2}, {"SOCK_RAW", 3}, {NULL, 0}};
static const named_argument target_args[] = {
    {"self", 1},         {"pgrp", 2},     {"others", 3}, {"children", 4},
    {"same-sandbox", 5}, {"initproc", 6}, {NULL, 0}};
static const named_argument system_attribute_args[] = {
    {"apple-internal", 0}, {"sandbox-debug-mode", 1}, {NULL, 0}};
static const named_argument vnode_type_args[] = {{"REGULAR-FILE", 1},
                                                 {"DIRECTORY", 2},
                                                 {"BLOCK-DEVICE", 3},
                                                 {"CHARACTER-DEVICE", 4},
                                                 {"SYMLINK", 5},
                                                 {"SOCKET", 6},
                                                 {"FIFO", 7},
                                                 {"TTY", 65535},
                                                 {NULL, 0}};
static const named_argument semaphore_owner_args[] = {{"self", 1},
                                                      {"pgrp", 2},
                                                      {"others", 3},
                                                      {"children", 4},
                                                      {"same-sandbox", 5},
                                                      {"initproc", 6},
                                                      {"platform-binary", 7},
                                                      {NULL, 0}};
static const named_argument process_attribute_args[] = {
    {"is-plugin", 0},   {"is-installer", 1},       {"is-restricted", 2},
    {"is-initproc", 3}, {"is-platform-binary", 4}, {NULL, 0}};
static const named_argument csr_args[] = {
    {"CSR_ALLOW_UNTRUSTED_KEXTS", 1},
    {"CSR_ALLOW_UNRESTRICTED_FS", 2},
    {"CSR_ALLOW_TASK_FOR_PID", 4},
    {"CSR_ALLOW_KERNEL_DEBUGGER", 8},
    {"CSR_ALLOW_APPLE_INTERNAL", 16},
    {"CSR_ALLOW_UNRESTRICTED_DTRACE", 32},
    {"CSR_ALLOW_UNRESTRICTED_NVRAM", 64},
    {"CSR_ALLOW_DEVICE_CONFIGURATION", 128},
    {NULL, 0}};
static const named_argument host_special_port_args[] = {
    {"HOST_DYNAMIC_PAGER_PORT", 8},
    {"HOST_AUDIT_CONTROL_PORT", 9},
    {"HOST_USER_NOTIFICATION_PORT", 10},
    {"HOST_AUTOMOUNTD_PORT", 11},
    {"HOST_LOCKD_PORT", 12},
    {"HOST_SEATBELT_PORT", 14},
    {"HOST_KEXTD_PORT", 15},
    {"HOST_CHUD_PORT", 16},
    {"HOST_UNFREED_PORT", 17},
    {"HOST_AMFID_PORT", 18},
    {"HOST_GSSD_PORT", 19},
    {"HOST_TELEMETRY_PORT", 20},
    {"HOST_ATM_NOTIFICATION_PORT", 21},
    {"HOST_COALITION_PORT", 22},
    {"HOST_SYSDIAGNOSE_PORT", 23},
    {"HOST_XPC_EXCEPTION_PORT", 24},
    {"HOST_CONTAINERD_PORT", 25},
    {NULL, 0}};
static const filter_info_t filter_infos[] = {
    {.name = NULL,
     .category = NULL,
     .argument_type = 0,
     .prerequisite = 0,
     .named_arguments = NULL},
    {
        .name = "path",
        .category = "path",
        .argument_type = SB_VALUE_TYPE_PATTERN_SUBPATH,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "mount-relative-path",
        .category = "path",
        .argument_type = SB_VALUE_TYPE_PATTERN_SUBPATH,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "xattr",
        .category = "xattr",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "file-mode",
        .category = "file-mode",
        .argument_type = SB_VALUE_TYPE_BITFIELD,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "ipc-posix-name",
        .category = "posix-ipc",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "global-name",
        .category = "mach",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "local-name",
        .category = "mach",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "local",
        .category = "network",
        .argument_type = SB_VALUE_TYPE_PATTERN_REGEX,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = local_args,
    },
    {
        .name = "remote",
        .category = "network",
        .argument_type = SB_VALUE_TYPE_PATTERN_REGEX,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = remote_args,
    },
    {
        .name = "control-name",
        .category = "network",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "socket-domain",
        .category = "socket",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = socket_domain_args,
    },
    {
        .name = "socket-type",
        .category = "socket",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = socket_type_args,
    },
    {
        .name = "socket-protocol",
        .category = "socket",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "target",
        .category = "target",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = target_args,
    },
    {
        .name = "fsctl-command",
        .category = "fsctl",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "ioctl-command",
        .category = "ioctl",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "iokit-registry-entry-class",
        .category = "iokit-registry-entry-class",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "iokit-property",
        .category = "iokit-property",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "iokit-connection",
        .category = "iokit-connection",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "device-major",
        .category = "device",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "device-minor",
        .category = "device",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "device-conforms-to",
        .category = "device",
        .argument_type = SB_VALUE_TYPE_STRING,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "extension",
        .category = "extension",
        .argument_type = SB_VALUE_TYPE_STRING,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "extension-class",
        .category = "extension-class",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "appleevent-destination",
        .category = "ae-destination",
        .argument_type = SB_VALUE_TYPE_PATTERN_PREFIX,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "system-attribute",
        .category = "system",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = system_attribute_args,
    },
    {
        .name = "right-name",
        .category = "auth-right-name",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "preference-domain",
        .category = "preference-domain",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "vnode-type",
        .category = "vnode-type",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = vnode_type_args,
    },
    {
        .name = "%entitlement-load",
        .category = "entitlement",
        .argument_type = SB_VALUE_TYPE_STRING,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "%entitlement-boolean",
        .category = "entitlement",
        .argument_type = SB_VALUE_TYPE_BOOL,
        .prerequisite = 30,
    },
    {
        .name = "%entitlement-string",
        .category = "entitlement",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = 30,
    },
    {
        .name = "kext-bundle-id",
        .category = "kext-bundle-id",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "info-type",
        .category = "info",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "notification-name",
        .category = "notification",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "notification-payload",
        .category = "notification",
        .argument_type = SB_VALUE_TYPE_BOOL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "semaphore-owner",
        .category = "semaphore",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = semaphore_owner_args,
    },
    {
        .name = "sysctl-name",
        .category = "sysctl",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "process-path",
        .category = "process",
        .argument_type = SB_VALUE_TYPE_PATTERN_SUBPATH,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "rootless-boot-device-filter",
        .category = "nvram",
        .argument_type = SB_VALUE_TYPE_BOOL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "rootless-file-filter",
        .category = "path",
        .argument_type = SB_VALUE_TYPE_BOOL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "rootless-disk-filter",
        .category = "path",
        .argument_type = SB_VALUE_TYPE_BOOL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "rootless-proc-filter",
        .category = "target",
        .argument_type = SB_VALUE_TYPE_BOOL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "privilege-id",
        .category = "priv",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "process-attribute",
        .category = "process",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = process_attribute_args,
    },
    {
        .name = "uid",
        .category = "process",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "nvram-variable",
        .category = "nvram",
        .argument_type = SB_VALUE_TYPE_PATTERN_PREFIX,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "csr",
        .category = "system",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = csr_args,
    },
    {
        .name = "host-special-port",
        .category = "mach-special-port",
        .argument_type = SB_VALUE_TYPE_INTEGER,
        .prerequisite = FILTER_NO_REQUISITE,
        .named_arguments = host_special_port_args,
    },
    {
        .name = "filesystem-name",
        .category = "fs",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
    {
        .name = "boot-arg",
        .category = "boot-arg",
        .argument_type = SB_VALUE_TYPE_PATTERN_LITERAL,
        .prerequisite = FILTER_NO_REQUISITE,
    },
};

/**
 * Check whether the specified filter accepts the specified type.
 * @param filter_info Information about the filter at hand
 * @param type The type to check
 * @return True, iff the specified filter accepts the type, false otherwise
 */
static bool sierra_filter_accepts_type(const filter_info_t *const filter,
                                       enum filter_argument_type type) {
  /*
   * The whole deal with the type of the arguments accepted by filters
   * are weird. There is a network type argument, but it is not used in
   * the definition of the network filters. However, this function implements
   * some funky logic...
   */
  switch (filter->argument_type) {
  case SB_VALUE_TYPE_BOOL:
    return type == SB_VALUE_TYPE_BOOL;
  case SB_VALUE_TYPE_BITFIELD:
    return type == SB_VALUE_TYPE_BITFIELD;
  case SB_VALUE_TYPE_INTEGER:
    return type == SB_VALUE_TYPE_INTEGER;
  case SB_VALUE_TYPE_STRING:
    return type == SB_VALUE_TYPE_STRING;
  case SB_VALUE_TYPE_PATTERN_LITERAL:
  case SB_VALUE_TYPE_PATTERN_PREFIX:
    return (type == SB_VALUE_TYPE_PATTERN_LITERAL) ||
           (type == SB_VALUE_TYPE_PATTERN_PREFIX) ||
           (type == SB_VALUE_TYPE_PATTERN_SUBPATH) ||
           (type == SB_VALUE_TYPE_REGEX);
  case SB_VALUE_TYPE_PATTERN_SUBPATH:
    return (type == SB_VALUE_TYPE_PATTERN_LITERAL) ||
           (type == SB_VALUE_TYPE_PATTERN_PREFIX) ||
           (type == SB_VALUE_TYPE_PATTERN_SUBPATH) ||
           (type == SB_VALUE_TYPE_PATTERN_REGEX) ||
           (type == SB_VALUE_TYPE_REGEX);
  case SB_VALUE_TYPE_PATTERN_REGEX:
    // -> this is rather weird, but exactly what Apple's code does.
    return type == SB_VALUE_TYPE_NETWORK;
  default:
    return false;
  }
}

const size_t sierra_filters(const filter_info_t **infos,
                            filter_accept_type_func *acceptor) {
  *infos = filter_infos;
  *acceptor = sierra_filter_accepts_type;

  return sizeof(filter_infos) / sizeof(*filter_infos);
}
