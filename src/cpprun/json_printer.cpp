#include "cpprun/json_printer.hpp"

#include <iostream>
#include <map>
#include <string>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "ocppi/runtime/features/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/state/types/Generators.hpp"    // IWYU pragma: keep
#include "ocppi/types/ContainerListItem.hpp"
#include "ocppi/types/Generators.hpp" // IWYU pragma: keep

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
} // namespace ocppi

namespace cpprun
{

void JSONPrinter::printFeatures(const ocppi::runtime::features::types::Features
                                        &features) const noexcept
{
        std::cout << nlohmann::json(features).dump();
}
void JSONPrinter::printState(
        const ocppi::runtime::state::types::State &state) const noexcept
{
        std::cout << nlohmann::json(state).dump();
}

void JSONPrinter::printContainerList(
        const std::vector<ocppi::types::ContainerListItem> &container_list)
        const noexcept
{
        std::cout << nlohmann::json(container_list).dump();
}

}
