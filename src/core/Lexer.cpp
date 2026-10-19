#include "Lexer.hpp"

#include <algorithm>
#include <cstdint>
#include <unordered_map>

#include "Diagnostic.hpp"
#include "Token.hpp"

namespace milan {

namespace {

const std::unordered_map<std::string_view, TokenKind> keywords = {
    {"program", TokenKind::Program},
    {"begin", TokenKind::Begin},
    {"end", TokenKind::End},
    {"if", TokenKind::If},
    {"then", TokenKind::Then},
    {"else", TokenKind::Else},
    {"while", TokenKind::While},
    {"do", TokenKind::Do},
    {"var", TokenKind::Var},
    {"const", TokenKind::Const},
    {"integer", TokenKind::Integer},
    {"boolean", TokenKind::Boolean},
    {"real", TokenKind::Real},
    {"function", TokenKind::Function},
    {"procedure", TokenKind::Procedure},
    {"forward", TokenKind::Forward},
    {"for", TokenKind::For},
    {"to", TokenKind::To},
    {"downto", TokenKind::Downto},
    {"div", TokenKind::Div},
    {"mod", TokenKind::Mod},
    {"and", TokenKind::And},
    {"or", TokenKind::Or},
    {"not", TokenKind::Not},
    {"xor", TokenKind::Xor},
    {"exit", TokenKind::Exit},
    {"break", TokenKind::Break},
    {"continue", TokenKind::Continue},
    {"array", TokenKind::Array},
    {"of", TokenKind::Of},
    {"true", TokenKind::True},
    {"false", TokenKind::False},
};

}

Lexer::Lexer(std::istream& input) : input(input) {}

Token Lexer::nextToken() {
    while (std::isspace(lastChar) || lastChar == '{') {
        if (lastChar == '{') {
            do {
                lastChar = readNextChar();
            } while (lastChar != '}' && lastChar != EOF);
        }

        lastChar = readNextChar();
    }

    SourceLocation tokLoc = currLoc;

    if (std::isalpha(lastChar) || lastChar == '_')
        return tokenizeKeywordOrIdentifier(tokLoc);
    if (std::isdigit(lastChar) || lastChar == '$' || lastChar == '&')
        return tokenizeNumber(tokLoc);
    if (lastChar == '\'') {
        std::string str;
        lastChar = readNextChar();
        while (lastChar != '\'' && lastChar != EOF) {
            str += static_cast<char>(lastChar);
            lastChar = readNextChar();
        }

        if (lastChar == '\'')
            lastChar = readNextChar();
        else
            Diagnostic::error(tokLoc, "Unterminated string literal");

        return Token{TokenKind::StringLiteral, tokLoc, str, 0};
    }
    if (lastChar == EOF)
        return Token{TokenKind::Eof, tokLoc, "", 0};

    return tokenizeOperator(tokLoc);
}

int Lexer::readNextChar() {
    int ch = input.get();

    if (ch == '\n') {
        ++currLoc.line;
        currLoc.column = 0;
    } else if (ch != EOF) {
        ++currLoc.column;
    }

    return ch;
}

bool Lexer::match(char expected) {
    if (lastChar == expected) {
        lastChar = readNextChar();
        return true;
    }
    return false;
}

Token Lexer::tokenizeKeywordOrIdentifier(SourceLocation loc) {
    std::string identifierStr;
    identifierStr += static_cast<char>(lastChar);

    while (std::isalnum(lastChar = readNextChar()) || lastChar == '_')
        identifierStr += static_cast<char>(lastChar);

    std::ranges::transform(identifierStr, identifierStr.begin(),
                           [](unsigned char c) { return std::tolower(c); });

    auto it = keywords.find(identifierStr);
    if (it != keywords.end())
        return Token{it->second, loc, identifierStr, 0};

    return Token{TokenKind::Identifier, loc, identifierStr, 0};
}

Token Lexer::tokenizeNumber(SourceLocation loc) {
    std::string numStr;
    int base = 10;

    if (match('$'))
        base = 16;
    else if (match('&'))
        base = 8;

    while (std::isalnum(lastChar)) {
        bool valid = false;

        if ((base == 10 && std::isdigit(lastChar)) || (base == 16 && std::isxdigit(lastChar)) ||
            (base == 8 && lastChar >= '0' && lastChar <= '7'))
            valid = true;

        if (!valid) {
            Diagnostic::error(loc, "Invalid digit");
            return Token{TokenKind::Error, loc, numStr, 0};
        }

        numStr += static_cast<char>(lastChar);
        lastChar = readNextChar();
    }

    bool isReal = false;
    if (base == 10 && lastChar == '.' && input.peek() != '.' && std::isdigit(input.peek())) {
        isReal = true;
        numStr += '.';
        lastChar = readNextChar();

        while (std::isdigit(lastChar)) {
            numStr += static_cast<char>(lastChar);
            lastChar = readNextChar();
        }
    }

    if (base == 10 && (lastChar == 'e' || lastChar == 'E')) {
        int nextCh = input.peek();
        if (std::isdigit(nextCh) || nextCh == '+' || nextCh == '-') {
            isReal = true;
            numStr += static_cast<char>(lastChar);
            lastChar = readNextChar();
            if (lastChar == '+' || lastChar == '-') {
                numStr += static_cast<char>(lastChar);
                lastChar = readNextChar();
            }
            while (std::isdigit(lastChar)) {
                numStr += static_cast<char>(lastChar);
                lastChar = readNextChar();
            }
        }
    }

    if (numStr.empty()) {
        Diagnostic::error(loc, "Empty numeric literal");
        return Token{TokenKind::Error, loc, numStr, 0};
    }

    if (isReal) {
        try {
            double realVal = std::stod(numStr);
            Token tok{TokenKind::RealNumber, loc, numStr, 0, realVal};
            return tok;
        } catch (...) {
            Diagnostic::error(loc, "Real number out of range");
            return Token{TokenKind::Error, loc, numStr, 0};
        }
    }

    try {
        std::int32_t numVal = std::stoi(numStr, nullptr, base);
        return Token{TokenKind::Number, loc, numStr, numVal};
    } catch (...) {
        Diagnostic::error(loc, "Number out of range");
        return Token{TokenKind::Error, loc, numStr, 0};
    }
}

Token Lexer::tokenizeOperator(SourceLocation loc) {
    auto makeToken = [&](TokenKind kind, std::string_view text) {
        return Token{kind, loc, std::string(text), 0};
    };

    char c = static_cast<char>(lastChar);
    lastChar = readNextChar();

    switch (c) {
        // Single character
        case '+':
            return makeToken(TokenKind::Plus, "+");
        case '-':
            return makeToken(TokenKind::Minus, "-");
        case '*':
            return makeToken(TokenKind::Mul, "*");
        case '/':
            return makeToken(TokenKind::Slash, "/");
        case '=':
            return makeToken(TokenKind::Equal, "=");
        case '(':
            return makeToken(TokenKind::LParen, "(");
        case ')':
            return makeToken(TokenKind::RParen, ")");
        case '[':
            return makeToken(TokenKind::LBracket, "[");
        case ']':
            return makeToken(TokenKind::RBracket, "]");
        case ',':
            return makeToken(TokenKind::Comma, ",");
        case ';':
            return makeToken(TokenKind::Semicolon, ";");
        // Double character
        case ':':
            return match('=') ? makeToken(TokenKind::Assign, ":=")
                              : makeToken(TokenKind::Colon, ":");
        case '>':
            return match('=') ? makeToken(TokenKind::GreaterEqual, ">=")
                              : makeToken(TokenKind::Greater, ">");
        case '.':
            return match('.') ? makeToken(TokenKind::Range, "..") : makeToken(TokenKind::Dot, ".");
        case '<':
            if (match('>'))
                return makeToken(TokenKind::NotEqual, "<>");
            if (match('='))
                return makeToken(TokenKind::LessEqual, "<=");
            return makeToken(TokenKind::Less, "<");
        default:
            Diagnostic::error(loc, "Unexpected character");
            return makeToken(TokenKind::Error, std::string(1, c));
    }
}

} // namespace milan
