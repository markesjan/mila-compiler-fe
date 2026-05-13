#pragma once

#include <iostream>
#include <unordered_map>

#include "Token.hpp"

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
    int lastChar = ' ';

    inline static const std::unordered_map<std::string, Token> keywords = {
        {"program", tok_program},
        {"begin", tok_begin},
        {"end", tok_end},
        {"if", tok_if},
        {"then", tok_then},
        {"else", tok_else},
        {"while", tok_while},
        {"do", tok_do},
        {"var", tok_var},
        {"const", tok_const},
        {"integer", tok_integer},
        {"boolean", tok_boolean},
        {"real", tok_real},
        {"function", tok_function},
        {"procedure", tok_procedure},
        {"for", tok_for},
        {"to", tok_to},
        {"downto", tok_downto},
        {"div", tok_div},
        {"mod", tok_mod},
        {"and", tok_and},
        {"or", tok_or},
        {"not", tok_not},
        {"xor", tok_xor},
        {"exit", tok_exit},
        {"break", tok_break},
        {"continue", tok_continue},
        {"array", tok_array},
        {"of", tok_of}
    };
};

} // namespace milan
