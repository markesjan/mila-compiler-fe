#include "Lexer.hpp"

#include <algorithm>

#include "Token.hpp"

namespace milan {

int Lexer::nextToken() {
    while (isspace(lastChar))
        lastChar = std::cin.get();

    if (isalpha(lastChar)) {
        identifierStr = lastChar;
        while (isalnum(lastChar = std::cin.get()))
            identifierStr += lastChar;

        std::transform(identifierStr.begin(), identifierStr.end(), identifierStr.begin(),
                       [](const char c) { return std::tolower(c); });

        auto it = keywords.find(identifierStr);
        if (it != keywords.end())
            return it->second;

        return tok_identifier;
    }

    if (isdigit(lastChar)) {

    }

    return tok_eof;
}

const std::string& Lexer::getIdentifierStr() const {
    return this->identifierStr;
}

int Lexer::getNumVal() const {
    return this->numVal;
}

} // namespace milan
