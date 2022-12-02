#pragma once

#include <filesystem>

#include <fmt/format.h>

#include "cpprun/exception.hpp"

namespace cpprun
{

auto root_path() -> std::filesystem::path
{
        auto xdg_runtime_dir_env = getenv("XDG_RUNTIME_DIR"); // NOLINT
        if (!xdg_runtime_dir_env) {
                if (getuid() != 0) {
                        throw_with_location(std::runtime_error(
                                "Couldn't decided which location use for root directory."));
                }
                return { "/run/cpprun" };
        }
        return { fmt::format("{}/cpprun", xdg_runtime_dir_env) };
}

}
