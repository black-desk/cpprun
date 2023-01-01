#include <mutex>

#include <black_desk/cpplib/logger.hpp>

#include "private_common.hpp"

namespace cxx_oci_spec::init
{

auto do_init_logger() noexcept -> int
{
        static std::once_flag once;
        std::call_once(once, []() {
                black_desk::cpplib::init_default_logger("cxx_oci_spec");
        });
        return 0;
}

} // namespace cxx_oci_spec::init
