#include "cpprun/text_printer.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "fmt/core.h"
#include "fmt/ranges.h" // IWYU pragma: keep
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "ocppi/runtime/features/types/Apparmor.hpp"
#include "ocppi/runtime/features/types/Cgroup.hpp"
#include "ocppi/runtime/features/types/Features.hpp"
#include "ocppi/runtime/features/types/IntelRdt.hpp"
#include "ocppi/runtime/features/types/Linux.hpp"
#include "ocppi/runtime/features/types/Seccomp.hpp"
#include "ocppi/runtime/features/types/Selinux.hpp"
#include "ocppi/runtime/state/types/State.hpp"
#include "ocppi/types/ContainerListItem.hpp"

using namespace std::string_view_literals;

namespace cpprun
{

void TextPrinter::printFeatures(const ocppi::runtime::features::types::Features
                                        &features) const noexcept
{
        std::cout << fmt::format("OCI version: [{},{})", features.ociVersionMin,
                                 features.ociVersionMax)
                  << std::endl;

        if (features.hooks) {
                std::cout << fmt::format("Support hooks: {}", *features.hooks)
                          << std::endl;
        }

        if (features.mountOptions) {
                std::cout << fmt::format("Support mount options: {}",
                                         *features.mountOptions)
                          << std::endl;
        }

        const auto linux_ = features.linux_.value_or(
                ocppi::runtime::features::types::Linux{});

        if (linux_.namespaces) {
                std::vector<std::string> namespaces;
                for (const auto &ns : *linux_.namespaces) {
                        namespaces.push_back(nlohmann::json(ns));
                }
                std::cout << fmt::format("Support namespaces: {}", namespaces)
                          << std::endl;
        }
        if (linux_.capabilities) {
                std::cout << fmt::format("Support capabilities: {}",
                                         *linux_.capabilities)
                          << std::endl;
        }

        const auto cgroup = linux_.cgroup.value_or(
                ocppi::runtime::features::types::Cgroup{});
        if (cgroup.v1) {
                std::cout << fmt::format("Support cgroup v1: {}", *cgroup.v1)
                          << std::endl;
        }
        if (cgroup.v2) {
                std::cout << fmt::format("Support cgroup v2: {}", *cgroup.v2)
                          << std::endl;
        }
        if (cgroup.systemd) {
                std::cout
                        << fmt::format("Support systemd cgroup controller: {}",
                                       *cgroup.systemd)
                        << std::endl;
        }
        if (cgroup.systemdUser) {
                std::cout
                        << fmt::format(
                                   "Support systemd user-scoped cgroup controller: {}",
                                   *cgroup.systemdUser)
                        << std::endl;
        }
        if (cgroup.rdma) {
                std::cout << fmt::format("Support RDMA cgroup controller: {}",
                                         *cgroup.rdma)
                          << std::endl;
        }

        const auto seccomp = linux_.seccomp.value_or(
                ocppi::runtime::features::types::Seccomp{});

        if (seccomp.enabled) {
                std::cout
                        << fmt::format("Support seccomp: {}", *seccomp.enabled)
                        << std::endl;
        }
        if (seccomp.actions) {
                std::vector<std::string> actions;
                for (const auto &action : *seccomp.actions) {
                        actions.push_back(nlohmann::json(action));
                }
                std::cout << fmt::format("Support seccomp actions: {}", actions)
                          << std::endl;
        }
        if (seccomp.operators) {
                std::vector<std::string> ops;
                for (const auto &op : *seccomp.operators) {
                        ops.push_back(nlohmann::json(op));
                }
                std::cout << fmt::format("Support seccomp operators: {}", ops)
                          << std::endl;
        }
        if (seccomp.archs) {
                std::vector<std::string> archs;
                for (const auto &arch : *seccomp.archs) {
                        archs.push_back(nlohmann::json(arch));
                }
                std::cout << fmt::format("Support seccomp architectures: {}",
                                         archs)
                          << std::endl;
        }
        if (seccomp.supportedFlags) {
                std::vector<std::string> flags;
                for (const auto &flag : *seccomp.supportedFlags) {
                        flags.push_back(nlohmann::json(flag));
                }
                std::cout << fmt::format("Support seccomp flags: {}", flags)
                          << std::endl;
        }

        const auto apparmor = linux_.apparmor.value_or(
                ocppi::runtime::features::types::Apparmor{});
        if (apparmor.enabled) {
                std::cout << fmt::format("Support apparmor: {}",
                                         *apparmor.enabled)
                          << std::endl;
        }

        const auto selinux = linux_.selinux.value_or(
                ocppi::runtime::features::types::Selinux{});
        if (selinux.enabled) {
                std::cout
                        << fmt::format("Support selinux: {}", *selinux.enabled)
                        << std::endl;
        }

        const auto intelRdt = linux_.intelRdt.value_or(
                ocppi::runtime::features::types::IntelRdt{});
        if (intelRdt.enabled) {
                std::cout << fmt::format("Support intel RDT: {}",
                                         *intelRdt.enabled)
                          << std::endl;
        }
}

void TextPrinter::printState(
        const ocppi::runtime::state::types::State &state) const noexcept
{
        std::cout << "OCI version: " << state.ociVersion << std::endl;
        std::cout << "id: " << state.id << std::endl;
        std::cout << "status: " << std::string(nlohmann::json(state.status))
                  << std::endl;
        if (state.pid) {
                std::cout << "PID: " << *state.pid << std::endl;
        }
        std::cout << "bundle: " << state.bundle << std::endl;
        if (state.annotations) {
                std::cout << "annotations:" << std::endl;
                for (const auto &anno : *state.annotations) {
                        std::cout << "\t" << anno.first << ": " << anno.second
                                  << std::endl;
                }
        }
}

void TextPrinter::printContainerList(
        const std::vector<ocppi::types::ContainerListItem> &container_list)
        const noexcept
{
        constexpr auto format = "{:<5} {:<5} {:<10} {:<30} {:<15} {:<10}"sv;
        std::cout << fmt::format(format, "ID", "PID", "STATUS", "BUNDLE",
                                 "CREATED", "OWNER")
                  << std::endl;
        for (const auto &container : container_list) {
                std::cout << fmt::format(format, container.id, container.pid,
                                         container.status, container.bundle,
                                         container.created, container.owner);
        }
}

}
