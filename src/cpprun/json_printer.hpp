#pragma once

#include <vector>

#include "cpprun/printer.hpp"

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
class JSONPrinter : public virtual Printer {
    public:
        void printFeatures(const ocppi::runtime::features::types::Features
                                   &features) const noexcept override;
        void printState(const ocppi::runtime::state::types::State &state)
                const noexcept override;
        void
        printContainerList(const std::vector<ocppi::types::ContainerListItem>
                                   &container_list) const noexcept override;
};
}
