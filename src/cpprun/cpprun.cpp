#include "cpprun/cpprun.hpp"

#include <map>
#include <optional>
#include <stdexcept>
#include <utility>

#include "cpprun/exception.hpp"
#include "ocppi/runtime/ContainerID.hpp"
#include "ocppi/runtime/CreateOption.hpp" // IWYU pragma: keep
#include "ocppi/runtime/DeleteOption.hpp" // IWYU pragma: keep
#include "ocppi/runtime/ExecOption.hpp"   // IWYU pragma: keep
#include "ocppi/runtime/GlobalOption.hpp" // IWYU pragma: keep
#include "ocppi/runtime/KillOption.hpp"   // IWYU pragma: keep
#include "ocppi/runtime/ListOption.hpp"   // IWYU pragma: keep
#include "ocppi/runtime/RunOption.hpp"    // IWYU pragma: keep
#include "ocppi/runtime/StartOption.hpp"  // IWYU pragma: keep
#include "ocppi/runtime/StateOption.hpp"  // IWYU pragma: keep
#include "ocppi/types/ContainerListItem.hpp"
#include "spdlog/spdlog.h"

namespace ocppi
{
namespace runtime
{
class Signal;
struct FeaturesOption;
} // namespace runtime
} // namespace ocppi

namespace cpprun
{

cpprun::cpprun(const std::unique_ptr<spdlog::logger> &logger,
               std::filesystem::path root)
        : logger(logger)
        , root(std::move(root))
{
        SPDLOG_LOGGER_TRACE(this->logger, "Using root directory {}.",
                            this->root.string());
}

auto cpprun::features() const noexcept
        -> tl::expected<ocppi::runtime::features::types::Features,
                        std::exception_ptr>
{
        return this->features({});
}

auto cpprun::features(
        const ocppi::runtime::FeaturesOption &option) const noexcept
        -> tl::expected<ocppi::runtime::features::types::Features,
                        std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::state(const ocppi::runtime::ContainerID &id) const noexcept
        -> tl::expected<ocppi::runtime::state::types::State, std::exception_ptr>
{
        return this->state(id, {});
}

auto cpprun::state(const ocppi::runtime::ContainerID &id,
                   const ocppi::runtime::StateOption &option) const noexcept
        -> tl::expected<ocppi::runtime::state::types::State, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::create(const ocppi::runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->create(id, pathToBundle, {});
}

auto cpprun::create(const ocppi::runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle,
                    const ocppi::runtime::CreateOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::start(const ocppi::runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->start(id, {});
}

auto cpprun::start(const ocppi::runtime::ContainerID &id,
                   const ocppi::runtime::StartOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::kill(const ocppi::runtime::ContainerID &id,
                  const ocppi::runtime::Signal &signal) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->kill(id, signal, {});
}

auto cpprun::kill(const ocppi::runtime::ContainerID &id,
                  const ocppi::runtime::Signal &signal,
                  const ocppi::runtime::KillOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::delete_(const ocppi::runtime::ContainerID &id) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->delete_(id, {});
}

auto cpprun::delete_(const ocppi::runtime::ContainerID &id,
                     const ocppi::runtime::DeleteOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::run(const ocppi::runtime::ContainerID &id,
                 const std::filesystem::path &pathToBundle) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->run(id, pathToBundle, {});
}

auto cpprun::run(const ocppi::runtime::ContainerID &id,
                 const std::filesystem::path &pathToBundle,
                 const ocppi::runtime::RunOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::exec(const ocppi::runtime::ContainerID &id,
                  const std::string &executable,
                  const std::vector<std::string> &command) noexcept
        -> tl::expected<void, std::exception_ptr>
{
        return this->exec(id, executable, command, {});
}

auto cpprun::exec(const ocppi::runtime::ContainerID &id,
                  const std::string &executable,
                  const std::vector<std::string> &command,
                  const ocppi::runtime::ExecOption &option) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto cpprun::list() noexcept
        -> tl::expected<std::vector<ocppi::types::ContainerListItem>,
                        std::exception_ptr>
{
        return this->list({});
}

auto cpprun::list(const ocppi::runtime::ListOption &option) noexcept
        -> tl::expected<std::vector<ocppi::types::ContainerListItem>,
                        std::exception_ptr>
try {
        throw_with_location(std::runtime_error("Not implemented yet"));
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
