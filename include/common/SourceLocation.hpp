#pragma once

#include <compare>
#include <cstdint>

namespace milan {

struct SourceLocation {
    std::uint32_t line = 1;
    std::uint32_t column = 0;

    constexpr auto operator<=>(const SourceLocation&) const = default;
};

} // namespace milan
