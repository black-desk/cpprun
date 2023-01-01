#include "capabilities.hpp"

#include <sys/capability.h>

#include "black_desk/cpplib/error.hpp"

namespace cxx_oci_spec
{

// NOLINTNEXTLINE
auto capability_string_to_int(const std::string_view &capability) -> int
{
#ifdef CAP_CHOWN
        if (capability == "CAP_CHOWN") {
                return CAP_CHOWN;
        }
#endif
#ifdef CAP_DAC_OVERRIDE
        if (capability == "CAP_DAC_OVERRIDE") {
                return CAP_DAC_OVERRIDE;
        }
#endif
#ifdef CAP_DAC_READ_SEARCH
        if (capability == "CAP_DAC_READ_SEARCH") {
                return CAP_DAC_READ_SEARCH;
        }
#endif
#ifdef CAP_FOWNER
        if (capability == "CAP_FOWNER") {
                return CAP_FOWNER;
        }
#endif
#ifdef CAP_FSETID
        if (capability == "CAP_FSETID") {
                return CAP_FSETID;
        }
#endif
#ifdef CAP_KILL
        if (capability == "CAP_KILL") {
                return CAP_KILL;
        }
#endif
#ifdef CAP_SETGID
        if (capability == "CAP_SETGID") {
                return CAP_SETGID;
        }
#endif
#ifdef CAP_SETUID
        if (capability == "CAP_SETUID") {
                return CAP_SETUID;
        }
#endif
#ifdef CAP_SETPCAP
        if (capability == "CAP_SETPCAP") {
                return CAP_SETPCAP;
        }
#endif
#ifdef CAP_LINUX_IMMUTABLE
        if (capability == "CAP_LINUX_IMMUTABLE") {
                return CAP_LINUX_IMMUTABLE;
        }
#endif
#ifdef CAP_NET_BIND_SERVICE
        if (capability == "CAP_NET_BIND_SERVICE") {
                return CAP_NET_BIND_SERVICE;
        }
#endif
#ifdef CAP_NET_BROADCAST
        if (capability == "CAP_NET_BROADCAST") {
                return CAP_NET_BROADCAST;
        }
#endif
#ifdef CAP_NET_ADMIN
        if (capability == "CAP_NET_ADMIN") {
                return CAP_NET_ADMIN;
        }
#endif
#ifdef CAP_NET_RAW
        if (capability == "CAP_NET_RAW") {
                return CAP_NET_RAW;
        }
#endif
#ifdef CAP_IPC_LOCK
        if (capability == "CAP_IPC_LOCK") {
                return CAP_IPC_LOCK;
        }
#endif
#ifdef CAP_IPC_OWNER
        if (capability == "CAP_IPC_OWNER") {
                return CAP_IPC_OWNER;
        }
#endif
#ifdef CAP_SYS_MODULE
        if (capability == "CAP_SYS_MODULE") {
                return CAP_SYS_MODULE;
        }
#endif
#ifdef CAP_SYS_RAWIO
        if (capability == "CAP_SYS_RAWIO") {
                return CAP_SYS_RAWIO;
        }
#endif
#ifdef CAP_SYS_CHROOT
        if (capability == "CAP_SYS_CHROOT") {
                return CAP_SYS_CHROOT;
        }
#endif
#ifdef CAP_SYS_PTRACE
        if (capability == "CAP_SYS_PTRACE") {
                return CAP_SYS_PTRACE;
        }
#endif
#ifdef CAP_SYS_PACCT
        if (capability == "CAP_SYS_PACCT") {
                return CAP_SYS_PACCT;
        }
#endif
#ifdef CAP_SYS_ADMIN
        if (capability == "CAP_SYS_ADMIN") {
                return CAP_SYS_ADMIN;
        }
#endif
#ifdef CAP_SYS_BOOT
        if (capability == "CAP_SYS_BOOT") {
                return CAP_SYS_BOOT;
        }
#endif
#ifdef CAP_SYS_NICE
        if (capability == "CAP_SYS_NICE") {
                return CAP_SYS_NICE;
        }
#endif
#ifdef CAP_SYS_RESOURCE
        if (capability == "CAP_SYS_RESOURCE") {
                return CAP_SYS_RESOURCE;
        }
#endif
#ifdef CAP_SYS_TIME
        if (capability == "CAP_SYS_TIME") {
                return CAP_SYS_TIME;
        }
#endif
#ifdef CAP_SYS_TTY_CONFIG
        if (capability == "CAP_SYS_TTY_CONFIG") {
                return CAP_SYS_TTY_CONFIG;
        }
#endif
#ifdef CAP_MKNOD
        if (capability == "CAP_MKNOD") {
                return CAP_MKNOD;
        }
#endif
#ifdef CAP_LEASE
        if (capability == "CAP_LEASE") {
                return CAP_LEASE;
        }
#endif
#ifdef CAP_AUDIT_WRITE
        if (capability == "CAP_AUDIT_WRITE") {
                return CAP_AUDIT_WRITE;
        }
#endif
#ifdef CAP_AUDIT_CONTROL
        if (capability == "CAP_AUDIT_CONTROL") {
                return CAP_AUDIT_CONTROL;
        }
#endif
#ifdef CAP_SETFCAP
        if (capability == "CAP_SETFCAP	") {
                return CAP_SETFCAP;
        }
#endif
#ifdef CAP_MAC_OVERRIDE
        if (capability == "CAP_MAC_OVERRIDE") {
                return CAP_MAC_OVERRIDE;
        }
#endif
#ifdef CAP_MAC_ADMIN
        if (capability == "CAP_MAC_ADMIN") {
                return CAP_MAC_ADMIN;
        }
#endif
#ifdef CAP_SYSLOG
        if (capability == "CAP_SYSLOG") {
                return CAP_SYSLOG;
        }
#endif
#ifdef CAP_WAKE_ALARM
        if (capability == "CAP_WAKE_ALARM") {
                return CAP_WAKE_ALARM;
        }
#endif
#ifdef CAP_BLOCK_SUSPEND
        if (capability == "CAP_BLOCK_SUSPEND") {
                return CAP_BLOCK_SUSPEND;
        }
#endif
#ifdef CAP_AUDIT_READ
        if (capability == "CAP_AUDIT_READ") {
                return CAP_AUDIT_READ;
        }
#endif
#ifdef CAP_PERFMON
        if (capability == "CAP_PERFMON") {
                return CAP_PERFMON;
        }
#endif
#ifdef CAP_BPF
        if (capability == "CAP_BPF") {
                return CAP_BPF;
        }
#endif
#ifdef CAP_CHECKPOINT_RESTORE
        if (capability == "CAP_CHECKPOINT_RESTORE") {
                return CAP_CHECKPOINT_RESTORE;
        }
#endif
        NESTED_EXCEPTION("unsupported capability \"{}\"", capability);
}

} // namespace cxx_oci_spec
