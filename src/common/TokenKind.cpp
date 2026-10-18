#include "TokenKind.hpp"

namespace milan {

std::string_view tokenKindToString(TokenKind kind) {
    switch (kind) {
        case TokenKind::Eof:
            return "end of file";
        case TokenKind::Error:
            return "error";
        case TokenKind::Identifier:
            return "identifier";
        case TokenKind::Number:
            return "number";
        case TokenKind::RealNumber:
            return "real number";
        case TokenKind::True:
            return "true";
        case TokenKind::False:
            return "false";
        case TokenKind::Program:
            return "program";
        case TokenKind::Begin:
            return "begin";
        case TokenKind::End:
            return "end";
        case TokenKind::Var:
            return "var";
        case TokenKind::Const:
            return "const";
        case TokenKind::Integer:
            return "integer";
        case TokenKind::Boolean:
            return "boolean";
        case TokenKind::Real:
            return "real";
        case TokenKind::Function:
            return "function";
        case TokenKind::Procedure:
            return "procedure";
        case TokenKind::Forward:
            return "forward";
        case TokenKind::If:
            return "if";
        case TokenKind::Then:
            return "then";
        case TokenKind::Else:
            return "else";
        case TokenKind::While:
            return "while";
        case TokenKind::Do:
            return "do";
        case TokenKind::For:
            return "for";
        case TokenKind::To:
            return "to";
        case TokenKind::Downto:
            return "downto";
        case TokenKind::Exit:
            return "exit";
        case TokenKind::Break:
            return "break";
        case TokenKind::Continue:
            return "continue";
        case TokenKind::Array:
            return "array";
        case TokenKind::Of:
            return "of";
        case TokenKind::Plus:
            return "+";
        case TokenKind::Minus:
            return "-";
        case TokenKind::Mul:
            return "*";
        case TokenKind::Slash:
            return "/";
        case TokenKind::Div:
            return "div";
        case TokenKind::Mod:
            return "mod";
        case TokenKind::And:
            return "and";
        case TokenKind::Or:
            return "or";
        case TokenKind::Not:
            return "not";
        case TokenKind::Xor:
            return "xor";
        case TokenKind::Equal:
            return "=";
        case TokenKind::NotEqual:
            return "<>";
        case TokenKind::Less:
            return "<";
        case TokenKind::LessEqual:
            return "<=";
        case TokenKind::Greater:
            return ">";
        case TokenKind::GreaterEqual:
            return ">=";
        case TokenKind::LParen:
            return "(";
        case TokenKind::RParen:
            return ")";
        case TokenKind::LBracket:
            return "[";
        case TokenKind::RBracket:
            return "]";
        case TokenKind::Comma:
            return ",";
        case TokenKind::Semicolon:
            return ";";
        case TokenKind::Colon:
            return ":";
        case TokenKind::Dot:
            return ".";
        case TokenKind::Assign:
            return ":=";
        case TokenKind::Range:
            return "..";
    }
    return "unknown token";
}

} // namespace milan
