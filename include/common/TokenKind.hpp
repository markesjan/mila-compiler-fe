/**
 * @file TokenKind.hpp
 * @brief Enumeration representing all lexical token kinds recognized by the Mila compiler.
 */

#pragma once

#include <cstdint>
#include <string_view>

namespace milan {

enum class TokenKind : std::uint8_t {
    Eof,
    Error,
    Identifier,
    Number,
    RealNumber,
    StringLiteral,
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
    Mul,
    Slash,
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

std::string_view tokenKindToString(TokenKind kind);

} // namespace milan
