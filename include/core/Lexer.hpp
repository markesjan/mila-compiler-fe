#pragma once

#include <iostream>

#include "SourceLocation.hpp"
#include "Token.hpp"

namespace milan {

class Lexer {
public:
    explicit Lexer(std::istream& input = std::cin);
    Token nextToken();

private:
    std::istream& input;
    int lastChar = ' ';
    SourceLocation currLoc{1, 0};

    int readNextChar();
    bool match(char expected);
    Token tokenizeKeywordOrIdentifier(SourceLocation loc);
    Token tokenizeNumber(SourceLocation loc);
    Token tokenizeOperator(SourceLocation loc);
};

} // namespace milan
