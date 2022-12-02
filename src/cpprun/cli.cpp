#include "cpprun/cli.hpp"

#include <exception>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "cpprun/configure.hpp"
#include "cpprun/cpprun.hpp"
#include "cpprun/documents.hpp"
#include "cpprun/exception.hpp"
#include "cpprun/json_printer.hpp"
#include "cpprun/paths.hpp"
#include "cpprun/text_printer.hpp"
#include "docopt_value.h"
#include "fmt/ranges.h"
#include "ocppi/runtime/Signal.hpp"
#include "ocppi/types/ContainerListItem.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "tl/expected.hpp"

namespace spdlog
{
class logger;
} // namespace spdlog

namespace cpprun
{

cli::cli(const std::vector<std::string> &arguments,
         const std::unique_ptr<spdlog::logger> &logger)
        : arguments(arguments)
        , logger(logger)
{
}

auto cli::execute() noexcept -> int
try {
        SPDLOG_LOGGER_TRACE(this->logger, "Execute with arguments {}.",
                            this->arguments);

        auto options = docopt::docopt(std::string(documents::readme),
                                      this->arguments, true, version, true);

        if (options["--json"].asBool()) {
                SPDLOG_LOGGER_TRACE(this->logger, "Using json printer.");
                this->printer = std::make_unique<JSONPrinter>();
        } else {
                SPDLOG_LOGGER_TRACE(this->logger, "Using text printer.");
                this->printer = std::make_unique<TextPrinter>();
        }

        std::string root_str;
        if (options["--root"].isString()) {
                root_str = options["--root"].asString();
        }
        auto root = root_str.empty() ? root_path() :
                                       std::filesystem::path(root_str);
        this->runtime = std::make_unique<cpprun>(this->logger, std::move(root));

        if (options["features"].asBool()) {
                return this->features(options);
        }
        if (options["state"].asBool()) {
                return this->state(options);
        }
        if (options["create"].asBool()) {
                return this->create(options);
        }
        if (options["start"].asBool()) {
                return this->start(options);
        }
        if (options["kill"].asBool()) {
                return this->kill(options);
        }
        if (options["delete"].asBool()) {
                return this->delete_(options);
        }
        if (options["list"].asBool()) {
                return this->list(options);
        }
        if (options["exec"].asBool()) {
                return this->exec(options);
        }
        if (options["run"].asBool()) {
                return this->run(options);
        }

        throw_with_location(std::logic_error("Unknown subcommand"));
} catch (...) {
        this->print_exception("Failed with", std::current_exception());
        return -1;
}

namespace
{

void print_nested_exception(std::ostringstream &out,
                            const std::exception &e) noexcept
{
        out << e.what() << '\n';
        try {
                std::rethrow_if_nested(e);
        } catch (const std::exception &exception) {
                print_nested_exception(out, exception);
        } catch (...) {
        }
}

auto parse_subcommand_option(std::string subcommand,
                             const std::string_view &help,
                             docopt::Options &options) noexcept
        -> docopt::Options
{
        std::vector<std::string> subcommand_arguments;
        if (options["<subcommand_args>"].isStringList()) {
                subcommand_arguments =
                        options["<subcommand_args>"].asStringList();
        }
        subcommand_arguments.insert(subcommand_arguments.begin(),
                                    std::move(subcommand));

        return docopt::docopt(std::string(help), subcommand_arguments, true,
                              "");
}

}

void cli::print_exception(const std::string &message,
                          const std::exception_ptr &ptr) const noexcept
{
        try {
                std::rethrow_exception(ptr);
        } catch (const std::exception &e) {
                std::ostringstream oss;
                print_nested_exception(oss, e);
                SPDLOG_LOGGER_ERROR(this->logger, "{} exception:\n{}", message,
                                    oss.str());
        } catch (...) {
                SPDLOG_LOGGER_ERROR(this->logger, "{} unknown exception.");
        }
}

auto cli::features(docopt::Options &options) const -> int
try {
        auto subcommand_options = parse_subcommand_option(
                "features", documents::features, options);

        auto feature = this->runtime->features();
        if (!feature) {
                rethrow_with_location(feature.error());
        }

        this->printer->printFeatures(*feature);

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `features`");
}

auto cli::state(docopt::Options &options) const -> int
try {
        auto subcommand_options =
                parse_subcommand_option("state", documents::state, options);

        auto container_id = subcommand_options["<container-id>"].asString();

        SPDLOG_LOGGER_TRACE(this->logger, "Query state of container {}",
                            container_id);

        auto state = this->runtime->state(container_id);
        if (!state) {
                rethrow_with_location(state.error());
        }

        this->printer->printState(*state);

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `state`");
}

auto cli::create(docopt::Options &options) -> int
try {
        auto subcommand_options =
                parse_subcommand_option("create", documents::create, options);

        auto result = this->runtime->create(
                subcommand_options["<container-id>"].asString(),
                subcommand_options["<path-to-bundle>"].asString());
        if (!result) {
                rethrow_with_location(result.error());
        }

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `create`");
}

auto cli::start(docopt::Options &options) -> int
try {
        auto subcommand_options =
                parse_subcommand_option("start", documents::start, options);

        auto result = this->runtime->start(
                subcommand_options["<container-id>"].asString());
        if (!result) {
                rethrow_with_location(result.error());
        }

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `start`");
}

auto cli::kill(docopt::Options &options) -> int
try {
        auto subcommand_options =
                parse_subcommand_option("kill", documents::kill, options);

        auto signal = ocppi::runtime::Signal(
                subcommand_options["<signal>"].asString().c_str());

        auto result = this->runtime->kill(
                subcommand_options["<container-id>"].asString(), signal);
        if (!result) {
                rethrow_with_location(result.error());
        }

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `kill`");
}

auto cli::delete_(docopt::Options &options) -> int
try {
        auto subcommand_options =
                parse_subcommand_option("delete", documents::delete_, options);

        auto result = this->runtime->delete_(
                subcommand_options["<container-id>"].asString());
        if (!result) {
                rethrow_with_location(result.error());
        }

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `delete`");
}

auto cli::list(docopt::Options &options) const -> int
try {
        auto subcommand_options =
                parse_subcommand_option("list", documents::list, options);

        auto list = this->runtime->list();
        if (!list) {
                rethrow_with_location(list.error());
        }

        this->printer->printContainerList(*list);

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `list`");
}

auto cli::exec(docopt::Options &options) -> int
try {
        auto subcommand_options =
                parse_subcommand_option("exec", documents::exec, options);

        auto result = this->runtime->exec(
                subcommand_options["<container-id>"].asString(),
                subcommand_options["<command>"].asStringList().front(),
                subcommand_options["<command>"].asStringList());
        if (!result) {
                rethrow_with_location(result.error());
        }

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `list`");
}

auto cli::run(docopt::Options &options) -> int
try {
        auto subcommand_options =
                parse_subcommand_option("run", documents::run, options);

        auto result = this->runtime->run(
                subcommand_options["<container-id>"].asString(),
                subcommand_options["<path-to-bundle>"].asString());
        if (!result) {
                rethrow_with_location(result.error());
        }

        return 0;
} catch (...) {
        rethrow_with_message("subcommand `list`");
}

}
