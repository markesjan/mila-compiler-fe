#include "Lexer.hpp"

namespace milan {

int Lexer::nextToken() {

}

const std::string& Lexer::getIdentifierStr() const {
    return this->identifierStr;
}

int Lexer::getNumVal() const {
    return this->numVal;
}

} // namespace milan
