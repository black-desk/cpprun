#include "rlimits.hpp"

#include <sys/resource.h>

#include "black_desk/cpplib/error.hpp"

namespace cxx_oci_spec
{

auto rlimit_string_to_int(const std::string_view &type) -> int
{
#ifdef RLIMIT_CPU
        if (type == "RLIMIT_CPU") {
                return RLIMIT_CPU;
        }
#endif
#ifdef RLIMIT_FSIZE
        if (type == "RLIMIT_FSIZE") {
                return RLIMIT_FSIZE;
        }
#endif
#ifdef RLIMIT_DATA
        if (type == "RLIMIT_DATA") {
                return RLIMIT_DATA;
        }
#endif
#ifdef RLIMIT_STACK
        if (type == "RLIMIT_STACK") {
                return RLIMIT_STACK;
        }
#endif
#ifdef RLIMIT_CORE
        if (type == "RLIMIT_CORE") {
                return RLIMIT_CORE;
        }
#endif
#ifdef RLIMIT_RSS
        if (type == "RLIMIT_RSS") {
                return RLIMIT_RSS;
        }
#endif
#ifdef RLIMIT_NOFILE
        if (type == "RLIMIT_NOFILE") {
                return RLIMIT_NOFILE;
        }
#endif
#ifdef RLIMIT_OFILE
        if (type == "RLIMIT_OFILE") {
                return RLIMIT_OFILE;
        }
#endif
#ifdef RLIMIT_AS
        if (type == "RLIMIT_AS") {
                return RLIMIT_AS;
        }
#endif
#ifdef RLIMIT_NPROC
        if (type == "RLIMIT_NPROC") {
                return RLIMIT_NPROC;
        }
#endif
#ifdef RLIMIT_MEMLOCK
        if (type == "RLIMIT_MEMLOCK") {
                return RLIMIT_MEMLOCK;
        }
#endif
#ifdef RLIMIT_LOCKS
        if (type == "RLIMIT_LOCKS") {
                return RLIMIT_LOCKS;
        }
#endif
#ifdef RLIMIT_SIGPENDING
        if (type == "RLIMIT_SIGPENDING") {
                return RLIMIT_SIGPENDING;
        }
#endif
#ifdef RLIMIT_MSGQUEUE
        if (type == "RLIMIT_MSGQUEUE") {
                return RLIMIT_MSGQUEUE;
        }
#endif
#ifdef RLIMIT_NICE
        if (type == "RLIMIT_NICE") {
                return RLIMIT_NICE;
        }
#endif
#ifdef RLIMIT_RTPRIO
        if (type == "RLIMIT_RTPRIO") {
                return RLIMIT_RTPRIO;
        }
#endif
#ifdef RLIMIT_RTTIME
        if (type == "RLIMIT_RTTIME") {
                return RLIMIT_RTTIME;
        }
#endif
        NESTED_EXCEPTION("unsupported rlimit type \"{}\"", type);
}

} // namespace cxx_oci_spec
