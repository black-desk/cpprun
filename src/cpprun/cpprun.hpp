#pragma once

#include <exception>
#include <filesystem>
#include <string>
#include <vector>

#include "ocppi/runtime/ContainerID.hpp"
#include "ocppi/runtime/Runtime.hpp"
#include "ocppi/runtime/features/types/Features.hpp"
#include "ocppi/runtime/state/types/State.hpp"
#include "ocppi/types/ContainerListItem.hpp"
#include "spdlog/logger.h"
#include "tl/expected.hpp"

namespace ocppi
{
namespace runtime
{
class Signal;
struct CreateOption;
struct DeleteOption;
struct ExecOption;
struct FeaturesOption;
struct KillOption;
struct ListOption;
struct RunOption;
struct StartOption;
struct StateOption;
} // namespace runtime
} // namespace ocppi

namespace cpprun
{
class cpprun : public virtual ocppi::runtime::Runtime {
    public:
        cpprun(const std::unique_ptr<spdlog::logger> &logger,
               std::filesystem::path root);

        [[nodiscard]]
        auto features() const noexcept
                -> tl::expected<ocppi::runtime::features::types::Features,
                                std::exception_ptr> override;
        [[nodiscard]]
        auto
        features(const ocppi::runtime::FeaturesOption &option) const noexcept
                -> tl::expected<ocppi::runtime::features::types::Features,
                                std::exception_ptr> override;

        [[nodiscard]]
        auto state(const ocppi::runtime::ContainerID &id) const noexcept
                -> tl::expected<ocppi::runtime::state::types::State,
                                std::exception_ptr> override;
        [[nodiscard]]
        auto state(const ocppi::runtime::ContainerID &id,
                   const ocppi::runtime::StateOption &option) const noexcept
                -> tl::expected<ocppi::runtime::state::types::State,
                                std::exception_ptr> override;

        [[nodiscard]]
        auto create(const ocppi::runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto create(const ocppi::runtime::ContainerID &id,
                    const std::filesystem::path &pathToBundle,
                    const ocppi::runtime::CreateOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto start(const ocppi::runtime::ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto start(const ocppi::runtime::ContainerID &id,
                   const ocppi::runtime::StartOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto kill(const ocppi::runtime::ContainerID &id,
                  const ocppi::runtime::Signal &signal) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto kill(const ocppi::runtime::ContainerID &id,
                  const ocppi::runtime::Signal &signal,
                  const ocppi::runtime::KillOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto delete_(const ocppi::runtime::ContainerID &id) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto delete_(const ocppi::runtime::ContainerID &id,
                     const ocppi::runtime::DeleteOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto run(const ocppi::runtime::ContainerID &id,
                 const std::filesystem::path &pathToBundle) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto run(const ocppi::runtime::ContainerID &id,
                 const std::filesystem::path &pathToBundle,
                 const ocppi::runtime::RunOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto exec(const ocppi::runtime::ContainerID &id,
                  const std::string &executable,
                  const std::vector<std::string> &command) noexcept
                -> tl::expected<void, std::exception_ptr> override;
        [[nodiscard]]
        auto exec(const ocppi::runtime::ContainerID &id,
                  const std::string &executable,
                  const std::vector<std::string> &command,
                  const ocppi::runtime::ExecOption &option) noexcept
                -> tl::expected<void, std::exception_ptr> override;

        [[nodiscard]]
        auto list() noexcept
                -> tl::expected<std::vector<ocppi::types::ContainerListItem>,
                                std::exception_ptr> override;
        [[nodiscard]]
        auto list(const ocppi::runtime::ListOption &option) noexcept
                -> tl::expected<std::vector<ocppi::types::ContainerListItem>,
                                std::exception_ptr> override;

    private:
        const std::unique_ptr<spdlog::logger> &logger;
        std::filesystem::path root;
};
}
