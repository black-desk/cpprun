#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "cpprun/printer.hpp"
#include "docopt.h"
#include "ocppi/runtime/Runtime.hpp"

namespace spdlog
{
class logger;
} // namespace spdlog

namespace cpprun
{
class cli {
    public:
        cli(const std::vector<std::string> &arguments,
            const std::unique_ptr<spdlog::logger> &logger);
        auto execute() noexcept -> int;

    private:
        void print_exception(const std::string &message,
                             const std::exception_ptr &ptr) const noexcept;

        int features(docopt::Options &options) const;
        int state(docopt::Options &options) const;
        int create(docopt::Options &options);
        int start(docopt::Options &options);
        int kill(docopt::Options &options);
        int delete_(docopt::Options &options);
        int list(docopt::Options &options) const;
        int exec(docopt::Options &options);
        int run(docopt::Options &options);

        const std::vector<std::string> &arguments;
        const std::unique_ptr<spdlog::logger> &logger;
        std::unique_ptr<ocppi::runtime::Runtime> runtime;
        std::unique_ptr<Printer> printer;
};
}
