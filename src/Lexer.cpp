#include "Lexer.hpp"

#include <algorithm>
#include <cstdint>

#include "Token.hpp"

namespace milan {

int Lexer::nextToken() {
    while (std::isspace(lastChar))
        lastChar = std::cin.get();

    if (std::isalpha(lastChar))
        return tokenizeKeyword();
    if (std::isdigit(lastChar) || lastChar == '$' || lastChar == '&')
        return tokenizeNumber();
    if (lastChar == ':' || lastChar == '<' || lastChar == '>')
        return tokenizeOperator();
    if (lastChar == EOF)
        return tok_eof;

    int currChar = lastChar;
    lastChar = std::cin.get();
    return currChar;
}

const std::string& Lexer::getIdentifierStr() const {
    return this->identifierStr;
}

int Lexer::getNumVal() const {
    return this->numVal;
}

int Lexer::tokenizeKeyword() {
    identifierStr = static_cast<char>(lastChar);
    while (std::isalnum(lastChar = std::cin.get()))
        identifierStr += static_cast<char>(lastChar);

    std::transform(identifierStr.begin(), identifierStr.end(), identifierStr.begin(),
                   [](const char c) { return std::tolower(c); });

    auto it = keywords.find(identifierStr);
    if (it != keywords.end())
        return it->second;

    return tok_identifier;
}

int Lexer::tokenizeNumber() {
    std::string numStr;
    int base = 10;

    if (lastChar == '$') {
        base = 16;
        lastChar = std::cin.get();
    } else if (lastChar == '&') {
        base = 8;
        lastChar = std::cin.get();
    }

    while (std::isalnum(lastChar)) {
        bool valid = false;

        if ((base == 10 && std::isdigit(lastChar)) ||
            (base == 16 && std::isxdigit(lastChar)) ||
            (base == 8 && lastChar >= '0' && lastChar <= '7'))
            valid = true;

        if (!valid) {
            std::cerr << "Lexical error: Invalid digit '" << static_cast<char>(lastChar) <<
                "' for base " << base << std::endl;
            return tok_error;
        }

        numStr += static_cast<char>(lastChar);
        lastChar = std::cin.get();
    }

    if (numStr.empty()) {
        std::cerr << "Lexical error: Empty numeric literal" << std::endl;
        return tok_error;
    }

    try {
        numVal = std::stoi(numStr, nullptr, base);
    } catch (...) {
        std::cerr << "Lexical error: Number out of range" << std::endl;
        return tok_error;
    }

    return tok_number;
}

int Lexer::tokenizeOperator() {
    if (lastChar == ':') {
        lastChar = std::cin.get();
        if (lastChar == '=') {
            lastChar = std::cin.get();
            return tok_assign;
        }
        return ':';
    }

    if (lastChar == '<') {
        lastChar = std::cin.get();
        if (lastChar == '>') {
            lastChar = std::cin.get();
            return tok_notequal;
        }
        if (lastChar == '=') {
            lastChar = std::cin.get();
            return tok_lessequal;
        }
        return '<';
    }

    if (lastChar == '>') {
        lastChar = std::cin.get();
        if (lastChar == '=') {
            lastChar = std::cin.get();
            return tok_greaterequal;
        }
        return '>';
    }

    return tok_error;
}

} // namespace milan
