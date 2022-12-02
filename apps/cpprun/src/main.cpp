#include <unistd.h>
#include <bits/types/struct_FILE.h>
#include <stdio.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "cpprun/cli.hpp"
#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/systemd_sink.h"
#include "spdlog/common.h"

namespace spdlog {
namespace sinks {
class sink;
}  // namespace sinks
}  // namespace spdlog

auto main(int argc, char **argv) noexcept -> int
{
        auto sinks = std::vector<std::shared_ptr<spdlog::sinks::sink>>(
                { std::make_shared<spdlog::sinks::systemd_sink_mt>("ocppi") });
        if (isatty(stderr->_fileno) != 0) {
                sinks.push_back(
                        std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
        }

        const auto logger = std::make_unique<spdlog::logger>(
                "cpprun", sinks.begin(), sinks.end());

        logger->set_level(spdlog::level::trace);

        std::vector<std::string> arguments;
        for (int i = 1; i < argc; i++) {
                arguments.emplace_back(argv[i]); // NOLINT
        }

        cpprun::cli cli(arguments, logger);

        return cli.execute();
}
