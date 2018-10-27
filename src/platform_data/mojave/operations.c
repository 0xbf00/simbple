#include "operations.h"

static const char *operation_names[] = {
    "default",
    "appleevent-send",
    "authorization-right-obtain",
    "boot-arg-set",
    "device*",
    "device-camera",
    "device-microphone",
    "darwin-notification-post",
    "distributed-notification-post",
    "dynamic-code-generation",
    "file*",
    "file-chroot",
    "file-clone",
    "file-ioctl",
    "file-issue-extension",
    "file-link",
    "file-map-executable",
    "file-mknod",
    "file-mount",
    "file-mount-update",
    "file-read*",
    "file-read-data",
    "file-read-metadata",
    "file-read-xattr",
    "file-revoke",
    "file-search",
    "file-test-existence",
    "file-unmount",
    "file-write*",
    "file-write-acl",
    "file-write-create",
    "file-write-data",
    "file-write-finderinfo",
    "file-write-flags",
    "file-write-mode",
    "file-write-owner",
    "file-write-setugid",
    "file-write-times",
    "file-write-unlink",
    "file-write-xattr",
    "fs-rename",
    "fs-snapshot*",
    "fs-snapshot-create",
    "fs-snapshot-delete",
    "fs-snapshot-revert",
    "generic-issue-extension",
    "qtn-user",
    "hid-control",
    "iokit*",
    "iokit-issue-extension",
    "iokit-open",
    "iokit-set-properties",
    "iokit-get-properties",
    "ipc*",
    "ipc-posix*",
    "ipc-posix-issue-extension",
    "ipc-posix-sem*",
    "ipc-posix-sem-create",
    "ipc-posix-sem-open",
    "ipc-posix-sem-post",
    "ipc-posix-sem-unlink",
    "ipc-posix-sem-wait",
    "ipc-posix-shm*",
    "ipc-posix-shm-read*",
    "ipc-posix-shm-read-data",
    "ipc-posix-shm-read-metadata",
    "ipc-posix-shm-write*",
    "ipc-posix-shm-write-create",
    "ipc-posix-shm-write-data",
    "ipc-posix-shm-write-unlink",
    "ipc-sysv*",
    "ipc-sysv-msg",
    "ipc-sysv-sem",
    "ipc-sysv-shm",
    "job-creation",
    "load-unsigned-code",
    "lsopen",
    "mach*",
    "mach-cross-domain-lookup",
    "mach-host*",
    "mach-host-exception-port-set",
    "mach-host-special-port-set",
    "mach-issue-extension",
    "mach-lookup",
    "mach-per-user-lookup",
    "mach-priv*",
    "mach-priv-host-port",
    "mach-priv-task-port",
    "mach-register",
    "mach-task-name",
    "network*",
    "network-inbound",
    "network-bind",
    "network-outbound",
    "nvram*",
    "nvram-delete",
    "nvram-get",
    "nvram-set",
    "opendirectory-user-modify",
    "process*",
    "process-codesigning-status*",
    "process-codesigning-status-set",
    "process-codesigning-status-get",
    "process-exec*",
    "process-exec-interpreter",
    "process-fork",
    "process-info*",
    "process-info-codesignature",
    "process-info-dirtycontrol",
    "process-info-listpids",
    "process-info-rusage",
    "process-info-pidinfo",
    "process-info-pidfdinfo",
    "process-info-pidfileportinfo",
    "process-info-setcontrol",
    "pseudo-tty",
    "signal",
    "socket-ioctl",
    "sysctl*",
    "sysctl-read",
    "sysctl-write",
    "system*",
    "system-acct",
    "system-audit",
    "system-automount",
    "system-chud",
    "system-debug",
    "system-fsctl",
    "system-info",
    "system-kext*",
    "system-kext-load",
    "system-kext-unload",
    "system-kext-query",
    "system-mac-label",
    "system-nfssvc",
    "system-package-check",
    "system-privilege",
    "system-reboot",
    "system-sched",
    "system-set-time",
    "system-socket",
    "system-suspend-resume",
    "system-swap",
    "user-preference*",
    "user-preference-read",
    "managed-preference-read",
    "user-preference-write",
    "storage-class-map",
};

static const char *default_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *default_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *appleevent_send_filters[] = {
    "extension",
    "process",
    "system",
    "ae-destination",
};
static const char *appleevent_send_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *authorization_right_obtain_filters[] = {
    "extension",
    "process",
    "system",
    "auth-right-name",
};
static const char *authorization_right_obtain_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *boot_arg_set_filters[] = {
    "extension",
    "process",
    "system",
    "boot-arg",
};
static const char *boot_arg_set_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *device_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *device_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *device_camera_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *device_camera_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *device_microphone_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *device_microphone_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *darwin_notification_post_filters[] = {
    "extension",
    "process",
    "system",
    "notification",
};
static const char *darwin_notification_post_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *distributed_notification_post_filters[] = {
    "extension",
    "process",
    "system",
    "notification",
};
static const char *distributed_notification_post_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *dynamic_code_generation_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *dynamic_code_generation_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_all_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_chroot_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_chroot_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_clone_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_clone_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_ioctl_filters[] = {
    "extension",  "process", "system", "path",  "file-mode",
    "vnode-type", "device",  "fs",     "ioctl",
};
static const char *file_ioctl_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_issue_extension_filters[] = {
    "extension", "process",   "system",
    "path",      "file-mode", "vnode-type",
    "device",    "fs",        "extension-class",
};
static const char *file_issue_extension_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_link_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_link_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_map_executable_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_map_executable_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_mknod_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_mknod_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_mount_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_mount_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_mount_update_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_mount_update_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_read_all_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_read_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_read_data_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_read_data_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_read_metadata_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_read_metadata_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
    "no-times",
};
static const char *file_read_xattr_filters[] = {
    "extension",  "process", "system", "path",  "file-mode",
    "vnode-type", "device",  "fs",     "xattr",
};
static const char *file_read_xattr_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_revoke_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_revoke_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_search_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_search_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_test_existence_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_test_existence_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_unmount_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_unmount_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_all_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_acl_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_acl_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_create_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_create_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_data_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_data_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_finderinfo_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_finderinfo_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_flags_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_flags_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_mode_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_mode_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_owner_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_owner_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_setugid_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_setugid_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_times_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_times_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_unlink_filters[] = {
    "extension", "process",    "system", "path",
    "file-mode", "vnode-type", "device", "fs",
};
static const char *file_write_unlink_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *file_write_xattr_filters[] = {
    "extension",  "process", "system", "path",  "file-mode",
    "vnode-type", "device",  "fs",     "xattr",
};
static const char *file_write_xattr_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *fs_rename_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *fs_rename_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *fs_snapshot_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *fs_snapshot_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *fs_snapshot_create_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *fs_snapshot_create_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *fs_snapshot_delete_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *fs_snapshot_delete_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *fs_snapshot_revert_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *fs_snapshot_revert_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *generic_issue_extension_filters[] = {
    "extension",
    "process",
    "system",
    "extension-class",
};
static const char *generic_issue_extension_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *qtn_user_filters[] = {
    "extension",
    "process",
    "system",
    "path",
};
static const char *qtn_user_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *hid_control_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *hid_control_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *iokit_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *iokit_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *iokit_issue_extension_filters[] = {
    "extension",
    "process",
    "system",
    "extension-class",
};
static const char *iokit_issue_extension_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *iokit_open_filters[] = {
    "extension", "process", "system", "iokit-user-client", "iokit-connection",
};
static const char *iokit_open_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *iokit_set_properties_filters[] = {
    "extension",      "process",           "system",
    "iokit-property", "iokit-user-client", "iokit-connection",
};
static const char *iokit_set_properties_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *iokit_get_properties_filters[] = {
    "extension",      "process",           "system",
    "iokit-property", "iokit-user-client", "iokit-connection",
};
static const char *iokit_get_properties_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *ipc_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_all_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_issue_extension_filters[] = {
    "extension", "process", "system", "posix-ipc", "extension-class",
};
static const char *ipc_posix_issue_extension_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_sem_all_filters[] = {
    "extension", "process", "system", "posix-ipc", "semaphore",
};
static const char *ipc_posix_sem_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_sem_create_filters[] = {
    "extension", "process", "system", "posix-ipc", "semaphore",
};
static const char *ipc_posix_sem_create_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_sem_open_filters[] = {
    "extension", "process", "system", "posix-ipc", "semaphore",
};
static const char *ipc_posix_sem_open_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_sem_post_filters[] = {
    "extension", "process", "system", "posix-ipc", "semaphore",
};
static const char *ipc_posix_sem_post_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_sem_unlink_filters[] = {
    "extension", "process", "system", "posix-ipc", "semaphore",
};
static const char *ipc_posix_sem_unlink_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_sem_wait_filters[] = {
    "extension", "process", "system", "posix-ipc", "semaphore",
};
static const char *ipc_posix_sem_wait_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_all_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_read_all_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_read_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_read_data_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_read_data_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_read_metadata_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_read_metadata_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_write_all_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_write_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_write_create_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_write_create_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_write_data_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_write_data_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_posix_shm_write_unlink_filters[] = {
    "extension",
    "process",
    "system",
    "posix-ipc",
};
static const char *ipc_posix_shm_write_unlink_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_sysv_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *ipc_sysv_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_sysv_msg_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *ipc_sysv_msg_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_sysv_sem_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *ipc_sysv_sem_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *ipc_sysv_shm_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *ipc_sysv_shm_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *job_creation_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *job_creation_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *load_unsigned_code_filters[] = {
    "extension", "process", "system", "path", "fs",
};
static const char *load_unsigned_code_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *lsopen_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *lsopen_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *mach_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_cross_domain_lookup_filters[] = {
    "extension",
    "process",
    "system",
    "mach",
};
static const char *mach_cross_domain_lookup_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_host_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *mach_host_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_host_exception_port_set_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *mach_host_exception_port_set_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_host_special_port_set_filters[] = {
    "extension",
    "process",
    "system",
    "mach-special-port",
};
static const char *mach_host_special_port_set_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_issue_extension_filters[] = {
    "extension", "process", "system", "mach", "extension-class",
};
static const char *mach_issue_extension_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_lookup_filters[] = {
    "extension",
    "process",
    "system",
    "mach",
};
static const char *mach_lookup_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_per_user_lookup_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *mach_per_user_lookup_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_priv_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *mach_priv_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_priv_host_port_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *mach_priv_host_port_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_priv_task_port_filters[] = {
    "extension", "process", "system", "path", "target",
};
static const char *mach_priv_task_port_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_register_filters[] = {
    "extension",
    "process",
    "system",
    "mach",
};
static const char *mach_register_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *mach_task_name_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *mach_task_name_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *network_all_filters[] = {
    "extension", "process",   "system",     "socket", "network",
    "path",      "file-mode", "vnode-type", "fs",
};
static const char *network_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *network_inbound_filters[] = {
    "extension", "process",   "system",     "socket", "network",
    "path",      "file-mode", "vnode-type", "fs",
};
static const char *network_inbound_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *network_bind_filters[] = {
    "extension", "process",   "system",     "socket", "network",
    "path",      "file-mode", "vnode-type", "fs",
};
static const char *network_bind_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *network_outbound_filters[] = {
    "extension", "process",   "system",     "socket", "network",
    "path",      "file-mode", "vnode-type", "fs",
};
static const char *network_outbound_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *nvram_all_filters[] = {
    "extension",
    "process",
    "system",
    "nvram",
};
static const char *nvram_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *nvram_delete_filters[] = {
    "extension",
    "process",
    "system",
    "nvram",
};
static const char *nvram_delete_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *nvram_get_filters[] = {
    "extension",
    "process",
    "system",
    "nvram",
};
static const char *nvram_get_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *nvram_set_filters[] = {
    "extension",
    "process",
    "system",
    "nvram",
};
static const char *nvram_set_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *opendirectory_user_modify_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *opendirectory_user_modify_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *process_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_codesigning_status_all_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_codesigning_status_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_codesigning_status_set_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_codesigning_status_set_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_codesigning_status_get_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_codesigning_status_get_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_exec_all_filters[] = {
    "extension", "process", "system", "path", "file-mode", "fs",
};
static const char *process_exec_all_modifiers[] = {
    "send-signal", "report",          "no-report",     "deprecated",
    "rootless",    "message",         "user-approval", "file-approval-override",
    "no-sandbox",  "default-sandbox",
};
static const char *process_exec_interpreter_filters[] = {
    "extension", "process", "system", "path", "file-mode", "fs",
};
static const char *process_exec_interpreter_modifiers[] = {
    "send-signal", "report",          "no-report",     "deprecated",
    "rootless",    "message",         "user-approval", "file-approval-override",
    "no-sandbox",  "default-sandbox",
};
static const char *process_fork_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *process_fork_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *process_info_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_codesignature_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_codesignature_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_dirtycontrol_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_dirtycontrol_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_listpids_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *process_info_listpids_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_rusage_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_rusage_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_pidinfo_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_pidinfo_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_pidfdinfo_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_pidfdinfo_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_pidfileportinfo_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_pidfileportinfo_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *process_info_setcontrol_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *process_info_setcontrol_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *pseudo_tty_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *pseudo_tty_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *signal_filters[] = {
    "extension", "process", "system", "target", "signal",
};
static const char *signal_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *socket_ioctl_filters[] = {
    "extension", "process", "system", "socket", "ioctl",
};
static const char *socket_ioctl_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *sysctl_all_filters[] = {
    "extension",
    "process",
    "system",
    "sysctl",
};
static const char *sysctl_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *sysctl_read_filters[] = {
    "extension",
    "process",
    "system",
    "sysctl",
};
static const char *sysctl_read_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *sysctl_write_filters[] = {
    "extension",
    "process",
    "system",
    "sysctl",
};
static const char *sysctl_write_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_acct_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_acct_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_audit_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_audit_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_automount_filters[] = {
    "extension",
    "process",
    "system",
    "path",
};
static const char *system_automount_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_chud_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_chud_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_debug_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *system_debug_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_fsctl_filters[] = {
    "extension",
    "process",
    "system",
    "fsctl",
};
static const char *system_fsctl_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_info_filters[] = {
    "extension",
    "process",
    "system",
    "info",
};
static const char *system_info_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_kext_all_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_kext_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_kext_load_filters[] = {
    "extension",
    "process",
    "system",
    "kext-bundle-id",
};
static const char *system_kext_load_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_kext_unload_filters[] = {
    "extension",
    "process",
    "system",
    "kext-bundle-id",
};
static const char *system_kext_unload_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_kext_query_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_kext_query_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_mac_label_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_mac_label_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_nfssvc_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_nfssvc_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_package_check_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_package_check_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_privilege_filters[] = {
    "extension",
    "process",
    "system",
    "priv",
};
static const char *system_privilege_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
    "grant",
};
static const char *system_reboot_filters[] = {
    "extension",
    "process",
    "system",
    "reboot-flags",
};
static const char *system_reboot_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_sched_filters[] = {
    "extension",
    "process",
    "system",
    "target",
};
static const char *system_sched_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_set_time_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_set_time_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_socket_filters[] = {
    "extension",
    "process",
    "system",
    "socket",
};
static const char *system_socket_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_suspend_resume_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_suspend_resume_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *system_swap_filters[] = {
    "extension",
    "process",
    "system",
};
static const char *system_swap_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *user_preference_all_filters[] = {
    "extension",
    "process",
    "system",
    "preference-domain",
};
static const char *user_preference_all_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *user_preference_read_filters[] = {
    "extension",
    "process",
    "system",
    "preference-domain",
};
static const char *user_preference_read_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *managed_preference_read_filters[] = {
    "extension",
    "process",
    "system",
    "preference-domain",
};
static const char *managed_preference_read_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *user_preference_write_filters[] = {
    "extension",
    "process",
    "system",
    "preference-domain",
};
static const char *user_preference_write_modifiers[] = {
    "send-signal", "report",  "no-report",     "deprecated",
    "rootless",    "message", "user-approval", "file-approval-override",
};
static const char *storage_class_map_filters[] = {
    "path",
};
static const char *storage_class_map_modifiers[] = {
    "assign-storage-class",
};

static const operation_info_t operation_infos[] = {
    {// default
     TERMINAL_NODE,
     0, // -> default
     0, default_filters, default_modifiers},
    {// appleevent-send
     NONTERMINAL_NODE,
     0, // -> default
     0, appleevent_send_filters, appleevent_send_modifiers},
    {// authorization-right-obtain
     NONTERMINAL_NODE,
     0, // -> default
     0, authorization_right_obtain_filters,
     authorization_right_obtain_modifiers},
    {// boot-arg-set
     NONTERMINAL_NODE,
     0, // -> default
     0, boot_arg_set_filters, boot_arg_set_modifiers},
    {// device*
     NONTERMINAL_NODE,
     0, // -> default
     0, device_all_filters, device_all_modifiers},
    {// device-camera
     NONTERMINAL_NODE,
     4, // -> device*
     0, device_camera_filters, device_camera_modifiers},
    {// device-microphone
     NONTERMINAL_NODE,
     4, // -> device*
     0, device_microphone_filters, device_microphone_modifiers},
    {// darwin-notification-post
     TERMINAL_NODE,
     0, // -> default
     1, darwin_notification_post_filters, darwin_notification_post_modifiers},
    {// distributed-notification-post
     NONTERMINAL_NODE,
     0, // -> default
     0, distributed_notification_post_filters,
     distributed_notification_post_modifiers},
    {// dynamic-code-generation
     TERMINAL_NODE,
     0, // -> default
     1, dynamic_code_generation_filters, dynamic_code_generation_modifiers},
    {// file*
     NONTERMINAL_NODE,
     0, // -> default
     0, file_all_filters, file_all_modifiers},
    {// file-chroot
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_chroot_filters, file_chroot_modifiers},
    {// file-clone
     TERMINAL_NODE,
     0, // -> default
     1, file_clone_filters, file_clone_modifiers},
    {// file-ioctl
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_ioctl_filters, file_ioctl_modifiers},
    {// file-issue-extension
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_issue_extension_filters, file_issue_extension_modifiers},
    {// file-link
     TERMINAL_NODE,
     0, // -> default
     1, file_link_filters, file_link_modifiers},
    {// file-map-executable
     TERMINAL_NODE,
     0, // -> default
     1, file_map_executable_filters, file_map_executable_modifiers},
    {// file-mknod
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_mknod_filters, file_mknod_modifiers},
    {// file-mount
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_mount_filters, file_mount_modifiers},
    {// file-mount-update
     NONTERMINAL_NODE,
     18, // -> file-mount
     0, file_mount_update_filters, file_mount_update_modifiers},
    {// file-read*
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_read_all_filters, file_read_all_modifiers},
    {// file-read-data
     NONTERMINAL_NODE,
     20, // -> file-read*
     0, file_read_data_filters, file_read_data_modifiers},
    {// file-read-metadata
     NONTERMINAL_NODE,
     20, // -> file-read*
     0, file_read_metadata_filters, file_read_metadata_modifiers},
    {// file-read-xattr
     NONTERMINAL_NODE,
     20, // -> file-read*
     0, file_read_xattr_filters, file_read_xattr_modifiers},
    {// file-revoke
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_revoke_filters, file_revoke_modifiers},
    {// file-search
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_search_filters, file_search_modifiers},
    {// file-test-existence
     TERMINAL_NODE,
     0, // -> default
     1, file_test_existence_filters, file_test_existence_modifiers},
    {// file-unmount
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_unmount_filters, file_unmount_modifiers},
    {// file-write*
     NONTERMINAL_NODE,
     10, // -> file*
     0, file_write_all_filters, file_write_all_modifiers},
    {// file-write-acl
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_acl_filters, file_write_acl_modifiers},
    {// file-write-create
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_create_filters, file_write_create_modifiers},
    {// file-write-data
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_data_filters, file_write_data_modifiers},
    {// file-write-finderinfo
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_finderinfo_filters, file_write_finderinfo_modifiers},
    {// file-write-flags
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_flags_filters, file_write_flags_modifiers},
    {// file-write-mode
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_mode_filters, file_write_mode_modifiers},
    {// file-write-owner
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_owner_filters, file_write_owner_modifiers},
    {// file-write-setugid
     TERMINAL_NODE,
     0, // -> default
     0, file_write_setugid_filters, file_write_setugid_modifiers},
    {// file-write-times
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_times_filters, file_write_times_modifiers},
    {// file-write-unlink
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_unlink_filters, file_write_unlink_modifiers},
    {// file-write-xattr
     NONTERMINAL_NODE,
     28, // -> file-write*
     0, file_write_xattr_filters, file_write_xattr_modifiers},
    {// fs-rename
     NONTERMINAL_NODE,
     0, // -> default
     0, fs_rename_filters, fs_rename_modifiers},
    {// fs-snapshot*
     NONTERMINAL_NODE,
     0, // -> default
     0, fs_snapshot_all_filters, fs_snapshot_all_modifiers},
    {// fs-snapshot-create
     NONTERMINAL_NODE,
     41, // -> fs-snapshot*
     0, fs_snapshot_create_filters, fs_snapshot_create_modifiers},
    {// fs-snapshot-delete
     NONTERMINAL_NODE,
     41, // -> fs-snapshot*
     0, fs_snapshot_delete_filters, fs_snapshot_delete_modifiers},
    {// fs-snapshot-revert
     NONTERMINAL_NODE,
     41, // -> fs-snapshot*
     0, fs_snapshot_revert_filters, fs_snapshot_revert_modifiers},
    {// generic-issue-extension
     NONTERMINAL_NODE,
     0, // -> default
     0, generic_issue_extension_filters, generic_issue_extension_modifiers},
    {// qtn-user
     NONTERMINAL_NODE,
     0, // -> default
     0, qtn_user_filters, qtn_user_modifiers},
    {// hid-control
     NONTERMINAL_NODE,
     0, // -> default
     0, hid_control_filters, hid_control_modifiers},
    {// iokit*
     NONTERMINAL_NODE,
     0, // -> default
     0, iokit_all_filters, iokit_all_modifiers},
    {// iokit-issue-extension
     NONTERMINAL_NODE,
     48, // -> iokit*
     0, iokit_issue_extension_filters, iokit_issue_extension_modifiers},
    {// iokit-open
     NONTERMINAL_NODE,
     48, // -> iokit*
     0, iokit_open_filters, iokit_open_modifiers},
    {// iokit-set-properties
     NONTERMINAL_NODE,
     48, // -> iokit*
     0, iokit_set_properties_filters, iokit_set_properties_modifiers},
    {// iokit-get-properties
     TERMINAL_NODE,
     0, // -> default
     1, iokit_get_properties_filters, iokit_get_properties_modifiers},
    {// ipc*
     NONTERMINAL_NODE,
     0, // -> default
     0, ipc_all_filters, ipc_all_modifiers},
    {// ipc-posix*
     NONTERMINAL_NODE,
     53, // -> ipc*
     0, ipc_posix_all_filters, ipc_posix_all_modifiers},
    {// ipc-posix-issue-extension
     NONTERMINAL_NODE,
     54, // -> ipc-posix*
     0, ipc_posix_issue_extension_filters, ipc_posix_issue_extension_modifiers},
    {// ipc-posix-sem*
     NONTERMINAL_NODE,
     54, // -> ipc-posix*
     0, ipc_posix_sem_all_filters, ipc_posix_sem_all_modifiers},
    {// ipc-posix-sem-create
     NONTERMINAL_NODE,
     56, // -> ipc-posix-sem*
     0, ipc_posix_sem_create_filters, ipc_posix_sem_create_modifiers},
    {// ipc-posix-sem-open
     NONTERMINAL_NODE,
     56, // -> ipc-posix-sem*
     0, ipc_posix_sem_open_filters, ipc_posix_sem_open_modifiers},
    {// ipc-posix-sem-post
     NONTERMINAL_NODE,
     56, // -> ipc-posix-sem*
     0, ipc_posix_sem_post_filters, ipc_posix_sem_post_modifiers},
    {// ipc-posix-sem-unlink
     NONTERMINAL_NODE,
     56, // -> ipc-posix-sem*
     0, ipc_posix_sem_unlink_filters, ipc_posix_sem_unlink_modifiers},
    {// ipc-posix-sem-wait
     NONTERMINAL_NODE,
     56, // -> ipc-posix-sem*
     0, ipc_posix_sem_wait_filters, ipc_posix_sem_wait_modifiers},
    {// ipc-posix-shm*
     NONTERMINAL_NODE,
     54, // -> ipc-posix*
     0, ipc_posix_shm_all_filters, ipc_posix_shm_all_modifiers},
    {// ipc-posix-shm-read*
     NONTERMINAL_NODE,
     62, // -> ipc-posix-shm*
     0, ipc_posix_shm_read_all_filters, ipc_posix_shm_read_all_modifiers},
    {// ipc-posix-shm-read-data
     NONTERMINAL_NODE,
     63, // -> ipc-posix-shm-read*
     0, ipc_posix_shm_read_data_filters, ipc_posix_shm_read_data_modifiers},
    {// ipc-posix-shm-read-metadata
     NONTERMINAL_NODE,
     63, // -> ipc-posix-shm-read*
     0, ipc_posix_shm_read_metadata_filters,
     ipc_posix_shm_read_metadata_modifiers},
    {// ipc-posix-shm-write*
     NONTERMINAL_NODE,
     62, // -> ipc-posix-shm*
     0, ipc_posix_shm_write_all_filters, ipc_posix_shm_write_all_modifiers},
    {// ipc-posix-shm-write-create
     NONTERMINAL_NODE,
     66, // -> ipc-posix-shm-write*
     0, ipc_posix_shm_write_create_filters,
     ipc_posix_shm_write_create_modifiers},
    {// ipc-posix-shm-write-data
     NONTERMINAL_NODE,
     66, // -> ipc-posix-shm-write*
     0, ipc_posix_shm_write_data_filters, ipc_posix_shm_write_data_modifiers},
    {// ipc-posix-shm-write-unlink
     NONTERMINAL_NODE,
     66, // -> ipc-posix-shm-write*
     0, ipc_posix_shm_write_unlink_filters,
     ipc_posix_shm_write_unlink_modifiers},
    {// ipc-sysv*
     NONTERMINAL_NODE,
     53, // -> ipc*
     0, ipc_sysv_all_filters, ipc_sysv_all_modifiers},
    {// ipc-sysv-msg
     NONTERMINAL_NODE,
     70, // -> ipc-sysv*
     0, ipc_sysv_msg_filters, ipc_sysv_msg_modifiers},
    {// ipc-sysv-sem
     NONTERMINAL_NODE,
     70, // -> ipc-sysv*
     0, ipc_sysv_sem_filters, ipc_sysv_sem_modifiers},
    {// ipc-sysv-shm
     NONTERMINAL_NODE,
     70, // -> ipc-sysv*
     0, ipc_sysv_shm_filters, ipc_sysv_shm_modifiers},
    {// job-creation
     TERMINAL_NODE,
     0, // -> default
     0, job_creation_filters, job_creation_modifiers},
    {// load-unsigned-code
     NONTERMINAL_NODE,
     0, // -> default
     0, load_unsigned_code_filters, load_unsigned_code_modifiers},
    {// lsopen
     NONTERMINAL_NODE,
     0, // -> default
     0, lsopen_filters, lsopen_modifiers},
    {// mach*
     NONTERMINAL_NODE,
     0, // -> default
     0, mach_all_filters, mach_all_modifiers},
    {// mach-cross-domain-lookup
     TERMINAL_NODE,
     0, // -> default
     1, mach_cross_domain_lookup_filters, mach_cross_domain_lookup_modifiers},
    {// mach-host*
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_host_all_filters, mach_host_all_modifiers},
    {// mach-host-exception-port-set
     NONTERMINAL_NODE,
     79, // -> mach-host*
     0, mach_host_exception_port_set_filters,
     mach_host_exception_port_set_modifiers},
    {// mach-host-special-port-set
     NONTERMINAL_NODE,
     79, // -> mach-host*
     0, mach_host_special_port_set_filters,
     mach_host_special_port_set_modifiers},
    {// mach-issue-extension
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_issue_extension_filters, mach_issue_extension_modifiers},
    {// mach-lookup
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_lookup_filters, mach_lookup_modifiers},
    {// mach-per-user-lookup
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_per_user_lookup_filters, mach_per_user_lookup_modifiers},
    {// mach-priv*
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_priv_all_filters, mach_priv_all_modifiers},
    {// mach-priv-host-port
     NONTERMINAL_NODE,
     85, // -> mach-priv*
     0, mach_priv_host_port_filters, mach_priv_host_port_modifiers},
    {// mach-priv-task-port
     NONTERMINAL_NODE,
     85, // -> mach-priv*
     0, mach_priv_task_port_filters, mach_priv_task_port_modifiers},
    {// mach-register
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_register_filters, mach_register_modifiers},
    {// mach-task-name
     NONTERMINAL_NODE,
     77, // -> mach*
     0, mach_task_name_filters, mach_task_name_modifiers},
    {// network*
     NONTERMINAL_NODE,
     0, // -> default
     0, network_all_filters, network_all_modifiers},
    {// network-inbound
     NONTERMINAL_NODE,
     90, // -> network*
     0, network_inbound_filters, network_inbound_modifiers},
    {// network-bind
     NONTERMINAL_NODE,
     91, // -> network-inbound
     0, network_bind_filters, network_bind_modifiers},
    {// network-outbound
     NONTERMINAL_NODE,
     90, // -> network*
     0, network_outbound_filters, network_outbound_modifiers},
    {// nvram*
     TERMINAL_NODE,
     0, // -> default
     1, nvram_all_filters, nvram_all_modifiers},
    {// nvram-delete
     NONTERMINAL_NODE,
     94, // -> nvram*
     0, nvram_delete_filters, nvram_delete_modifiers},
    {// nvram-get
     NONTERMINAL_NODE,
     94, // -> nvram*
     0, nvram_get_filters, nvram_get_modifiers},
    {// nvram-set
     NONTERMINAL_NODE,
     94, // -> nvram*
     0, nvram_set_filters, nvram_set_modifiers},
    {// opendirectory-user-modify
     NONTERMINAL_NODE,
     0, // -> default
     0, opendirectory_user_modify_filters, opendirectory_user_modify_modifiers},
    {// process*
     NONTERMINAL_NODE,
     0, // -> default
     0, process_all_filters, process_all_modifiers},
    {// process-codesigning-status*
     NONTERMINAL_NODE,
     99, // -> process*
     0, process_codesigning_status_all_filters,
     process_codesigning_status_all_modifiers},
    {// process-codesigning-status-set
     NONTERMINAL_NODE,
     100, // -> process-codesigning-status*
     0, process_codesigning_status_set_filters,
     process_codesigning_status_set_modifiers},
    {// process-codesigning-status-get
     NONTERMINAL_NODE,
     100, // -> process-codesigning-status*
     0, process_codesigning_status_get_filters,
     process_codesigning_status_get_modifiers},
    {// process-exec*
     NONTERMINAL_NODE,
     99, // -> process*
     0, process_exec_all_filters, process_exec_all_modifiers},
    {// process-exec-interpreter
     NONTERMINAL_NODE,
     103, // -> process-exec*
     0, process_exec_interpreter_filters, process_exec_interpreter_modifiers},
    {// process-fork
     NONTERMINAL_NODE,
     99, // -> process*
     0, process_fork_filters, process_fork_modifiers},
    {// process-info*
     TERMINAL_NODE,
     0, // -> default
     1, process_info_all_filters, process_info_all_modifiers},
    {// process-info-codesignature
     TERMINAL_NODE,
     0, // -> default
     1, process_info_codesignature_filters,
     process_info_codesignature_modifiers},
    {// process-info-dirtycontrol
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_dirtycontrol_filters, process_info_dirtycontrol_modifiers},
    {// process-info-listpids
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_listpids_filters, process_info_listpids_modifiers},
    {// process-info-rusage
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_rusage_filters, process_info_rusage_modifiers},
    {// process-info-pidinfo
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_pidinfo_filters, process_info_pidinfo_modifiers},
    {// process-info-pidfdinfo
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_pidfdinfo_filters, process_info_pidfdinfo_modifiers},
    {// process-info-pidfileportinfo
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_pidfileportinfo_filters,
     process_info_pidfileportinfo_modifiers},
    {// process-info-setcontrol
     NONTERMINAL_NODE,
     106, // -> process-info*
     0, process_info_setcontrol_filters, process_info_setcontrol_modifiers},
    {// pseudo-tty
     NONTERMINAL_NODE,
     0, // -> default
     0, pseudo_tty_filters, pseudo_tty_modifiers},
    {// signal
     NONTERMINAL_NODE,
     0, // -> default
     0, signal_filters, signal_modifiers},
    {// socket-ioctl
     TERMINAL_NODE,
     0, // -> default
     1, socket_ioctl_filters, socket_ioctl_modifiers},
    {// sysctl*
     NONTERMINAL_NODE,
     0, // -> default
     0, sysctl_all_filters, sysctl_all_modifiers},
    {// sysctl-read
     NONTERMINAL_NODE,
     118, // -> sysctl*
     0, sysctl_read_filters, sysctl_read_modifiers},
    {// sysctl-write
     NONTERMINAL_NODE,
     118, // -> sysctl*
     0, sysctl_write_filters, sysctl_write_modifiers},
    {// system*
     NONTERMINAL_NODE,
     0, // -> default
     0, system_all_filters, system_all_modifiers},
    {// system-acct
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_acct_filters, system_acct_modifiers},
    {// system-audit
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_audit_filters, system_audit_modifiers},
    {// system-automount
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_automount_filters, system_automount_modifiers},
    {// system-chud
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_chud_filters, system_chud_modifiers},
    {// system-debug
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_debug_filters, system_debug_modifiers},
    {// system-fsctl
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_fsctl_filters, system_fsctl_modifiers},
    {// system-info
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_info_filters, system_info_modifiers},
    {// system-kext*
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_kext_all_filters, system_kext_all_modifiers},
    {// system-kext-load
     NONTERMINAL_NODE,
     129, // -> system-kext*
     0, system_kext_load_filters, system_kext_load_modifiers},
    {// system-kext-unload
     NONTERMINAL_NODE,
     129, // -> system-kext*
     0, system_kext_unload_filters, system_kext_unload_modifiers},
    {// system-kext-query
     NONTERMINAL_NODE,
     129, // -> system-kext*
     0, system_kext_query_filters, system_kext_query_modifiers},
    {// system-mac-label
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_mac_label_filters, system_mac_label_modifiers},
    {// system-nfssvc
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_nfssvc_filters, system_nfssvc_modifiers},
    {// system-package-check
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_package_check_filters, system_package_check_modifiers},
    {// system-privilege
     TERMINAL_NODE,
     0, // -> default
     1, system_privilege_filters, system_privilege_modifiers},
    {// system-reboot
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_reboot_filters, system_reboot_modifiers},
    {// system-sched
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_sched_filters, system_sched_modifiers},
    {// system-set-time
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_set_time_filters, system_set_time_modifiers},
    {// system-socket
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_socket_filters, system_socket_modifiers},
    {// system-suspend-resume
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_suspend_resume_filters, system_suspend_resume_modifiers},
    {// system-swap
     NONTERMINAL_NODE,
     121, // -> system*
     0, system_swap_filters, system_swap_modifiers},
    {// user-preference*
     NONTERMINAL_NODE,
     0, // -> default
     0, user_preference_all_filters, user_preference_all_modifiers},
    {// user-preference-read
     NONTERMINAL_NODE,
     143, // -> user-preference*
     0, user_preference_read_filters, user_preference_read_modifiers},
    {// managed-preference-read
     NONTERMINAL_NODE,
     144, // -> user-preference-read
     0, managed_preference_read_filters, managed_preference_read_modifiers},
    {// user-preference-write
     NONTERMINAL_NODE,
     143, // -> user-preference*
     0, user_preference_write_filters, user_preference_write_modifiers},
    {// storage-class-map
     TERMINAL_NODE,
     0, // -> default
     0, storage_class_map_filters, storage_class_map_modifiers},
};

const size_t mojave_ops(const char ***names, const operation_info_t **infos) {
  *names = operation_names;
  *infos = operation_infos;

  return sizeof(operation_names) / sizeof(*operation_names);
}