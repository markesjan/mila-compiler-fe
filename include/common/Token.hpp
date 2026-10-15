#pragma once

#include <string>

#include "SourceLocation.hpp"

namespace milan {

enum class TokenKind : std::uint8_t {
    Eof,
    Error,
    Identifier,
    Number,
    True,
    False,
    Program,
    Begin,
    End,
    Var,
    Const,
    Integer,
    Boolean,
    Real,
    Function,
    Procedure,
    Forward,
    If,
    Then,
    Else,
    While,
    Do,
    For,
    To,
    Downto,
    Exit,
    Break,
    Continue,
    Array,
    Of,
    Plus,
    Minus,
    Star,
    Div,
    Mod,
    And,
    Or,
    Not,
    Xor,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    LParen,
    RParen,
    LBracket,
    RBracket,
    Comma,
    Semicolon,
    Colon,
    Dot,
    Assign,
    Range
};

struct Token {
    TokenKind kind = TokenKind::Eof;
    SourceLocation location{};
    std::string text;
    std::int32_t numberValue = 0;

    constexpr bool is(TokenKind expected) const noexcept { return kind == expected; }

    constexpr bool isNot(TokenKind expected) const noexcept { return kind != expected; }
};

} // namespace milan
