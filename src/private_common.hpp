#pragma once

#include <black_desk/cpplib/error.hpp>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

namespace cpprun::init
{
auto do_init_logger() noexcept -> int;
static const int init_logger = do_init_logger();
}
