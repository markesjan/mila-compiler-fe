#pragma once

#include <iostream>

namespace milan {

class Lexer {
public:
    Lexer() = default;
    ~Lexer() = default;

    int nextToken();

    const std::string& getIdentifierStr() const;
    int getNumVal() const;

private:
    std::string identifierStr;
    int numVal;
};

} // namespace milan
