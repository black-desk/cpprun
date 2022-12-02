#pragma once

#include <vector>

namespace ocppi
{
namespace runtime
{
namespace features
{
namespace types
{
struct Features;
} // namespace types
} // namespace features
namespace state
{
namespace types
{
struct State;
} // namespace types
} // namespace state
} // namespace runtime
namespace types
{
struct ContainerListItem;
} // namespace types
} // namespace ocppi

namespace cpprun
{
class Printer {
    public:
        Printer() = default;
        Printer(const Printer &) = delete;
        Printer(Printer &&) = delete;
        Printer &operator=(const Printer &) = delete;
        Printer &operator=(Printer &&) = delete;
        virtual ~Printer() = default;

        virtual void
        printFeatures(const ocppi::runtime::features::types::Features &features)
                const noexcept = 0;
        virtual void printState(const ocppi::runtime::state::types::State
                                        &state) const noexcept = 0;
        virtual void
        printContainerList(const std::vector<ocppi::types::ContainerListItem>
                                   &container_list) const noexcept = 0;
};
}
