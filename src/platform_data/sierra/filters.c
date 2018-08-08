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
    {NULL, NULL, 0, 0, NULL},
    {"path", "path", SB_VALUE_TYPE_PATTERN_SUBPATH, FILTER_NO_REQUISITE, NULL},
    {"mount-relative-path", "path", SB_VALUE_TYPE_PATTERN_SUBPATH,
     FILTER_NO_REQUISITE, NULL},
    {"xattr", "xattr", SB_VALUE_TYPE_PATTERN_LITERAL, FILTER_NO_REQUISITE,
     NULL},
    {"file-mode", "file-mode", SB_VALUE_TYPE_BITFIELD, FILTER_NO_REQUISITE,
     NULL},
    {"ipc-posix-name", "posix-ipc", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"global-name", "mach", SB_VALUE_TYPE_PATTERN_LITERAL, FILTER_NO_REQUISITE,
     NULL},
    {"local-name", "mach", SB_VALUE_TYPE_PATTERN_LITERAL, FILTER_NO_REQUISITE,
     NULL},
    {
        "local",
        "network",
        SB_VALUE_TYPE_PATTERN_REGEX,
        FILTER_NO_REQUISITE,
        local_args,
    },
    {
        "remote",
        "network",
        SB_VALUE_TYPE_PATTERN_REGEX,
        FILTER_NO_REQUISITE,
        remote_args,
    },
    {"control-name", "network", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {
        "socket-domain",
        "socket",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        socket_domain_args,
    },
    {
        "socket-type",
        "socket",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        socket_type_args,
    },
    {"socket-protocol", "socket", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE,
     NULL},
    {
        "target",
        "target",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        target_args,
    },
    {"fsctl-command", "fsctl", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE,
     NULL},
    {"ioctl-command", "ioctl", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE,
     NULL},
    {"iokit-registry-entry-class", "iokit-registry-entry-class",
     SB_VALUE_TYPE_PATTERN_LITERAL, FILTER_NO_REQUISITE, NULL},
    {"iokit-property", "iokit-property", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"iokit-connection", "iokit-connection", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"device-major", "device", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE,
     NULL},
    {"device-minor", "device", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE,
     NULL},
    {"device-conforms-to", "device", SB_VALUE_TYPE_STRING, FILTER_NO_REQUISITE,
     NULL},
    {"extension", "extension", SB_VALUE_TYPE_STRING, FILTER_NO_REQUISITE, NULL},
    {"extension-class", "extension-class", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"appleevent-destination", "ae-destination", SB_VALUE_TYPE_PATTERN_PREFIX,
     FILTER_NO_REQUISITE, NULL},
    {
        "system-attribute",
        "system",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        system_attribute_args,
    },
    {"right-name", "auth-right-name", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"preference-domain", "preference-domain", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {
        "vnode-type",
        "vnode-type",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        vnode_type_args,
    },
    {"%entitlement-load", "entitlement", SB_VALUE_TYPE_STRING,
     FILTER_NO_REQUISITE, NULL},
    {"%entitlement-boolean", "entitlement", SB_VALUE_TYPE_BOOL, 30, NULL},
    {"%entitlement-string", "entitlement", SB_VALUE_TYPE_PATTERN_LITERAL, 30,
     NULL},
    {"kext-bundle-id", "kext-bundle-id", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"info-type", "info", SB_VALUE_TYPE_PATTERN_LITERAL, FILTER_NO_REQUISITE,
     NULL},
    {"notification-name", "notification", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"notification-payload", "notification", SB_VALUE_TYPE_BOOL,
     FILTER_NO_REQUISITE, NULL},
    {
        "semaphore-owner",
        "semaphore",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        semaphore_owner_args,
    },
    {"sysctl-name", "sysctl", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"process-path", "process", SB_VALUE_TYPE_PATTERN_SUBPATH,
     FILTER_NO_REQUISITE, NULL},
    {"rootless-boot-device-filter", "nvram", SB_VALUE_TYPE_BOOL,
     FILTER_NO_REQUISITE, NULL},
    {"rootless-file-filter", "path", SB_VALUE_TYPE_BOOL, FILTER_NO_REQUISITE,
     NULL},
    {"rootless-disk-filter", "path", SB_VALUE_TYPE_BOOL, FILTER_NO_REQUISITE,
     NULL},
    {"rootless-proc-filter", "target", SB_VALUE_TYPE_BOOL, FILTER_NO_REQUISITE,
     NULL},
    {"privilege-id", "priv", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE, NULL},
    {
        "process-attribute",
        "process",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        process_attribute_args,
    },
    {"uid", "process", SB_VALUE_TYPE_INTEGER, FILTER_NO_REQUISITE, NULL},
    {"nvram-variable", "nvram", SB_VALUE_TYPE_PATTERN_PREFIX,
     FILTER_NO_REQUISITE, NULL},
    {
        "csr",
        "system",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        csr_args,
    },
    {
        "host-special-port",
        "mach-special-port",
        SB_VALUE_TYPE_INTEGER,
        FILTER_NO_REQUISITE,
        host_special_port_args,
    },
    {"filesystem-name", "fs", SB_VALUE_TYPE_PATTERN_LITERAL,
     FILTER_NO_REQUISITE, NULL},
    {"boot-arg", "boot-arg", SB_VALUE_TYPE_PATTERN_LITERAL, FILTER_NO_REQUISITE,
     NULL},
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