#pragma once

#include <exception>
#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>

#include "fmt/core.h"

namespace cpprun
{
[[noreturn]]
inline void rethrow_with_message(
        std::string &&message,
        std::exception_ptr eptr = std::current_exception(),
        const std::source_location location = std::source_location::current())
{
        try {
                std::rethrow_exception(std::move(eptr));
        } catch (...) {
                std::throw_with_nested(std::runtime_error(fmt::format(
                        "[{}:{} {}] {}", location.file_name(), location.line(),
                        location.function_name(), std::move(message))));
        }
}

[[noreturn]]
inline void rethrow_with_location(
        std::exception_ptr eptr = std::current_exception(),
        const std::source_location location = std::source_location::current())
{
        try {
                std::rethrow_exception(std::move(eptr));
        } catch (...) {
                std::throw_with_nested(std::runtime_error(fmt::format(
                        "[{}:{} {}]", location.file_name(), location.line(),
                        location.function_name())));
        }
}

template <typename Exception>
[[noreturn]]
inline void throw_with_location(
        Exception &&exception,
        std::source_location location = std::source_location::current())
try {
        static_assert(std::is_base_of_v<std::exception, Exception>);
        throw std::forward<Exception>(exception);
} catch (...) {
        rethrow_with_location(std::current_exception(), location);
}

}
