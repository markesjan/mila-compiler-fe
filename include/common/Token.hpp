#pragma once

#include <string>
#include <string_view>

#include "SourceLocation.hpp"
#include "TokenKind.hpp"

namespace milan {

struct Token {
    TokenKind kind = TokenKind::Eof;
    SourceLocation location{};
    std::string text;
    std::int32_t numberValue = 0;

    std::string_view name() const noexcept;

    constexpr bool is(TokenKind expected) const noexcept { return kind == expected; }

    constexpr bool isNot(TokenKind expected) const noexcept { return kind != expected; }
};

} // namespace milan
