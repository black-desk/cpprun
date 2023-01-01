#pragma once

#include <string_view>
namespace cxx_oci_spec
{
auto rlimit_string_to_int(const std::string_view &type) -> int;
}
