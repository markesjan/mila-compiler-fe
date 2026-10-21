#include "Parser.hpp"

#include <algorithm>
#include <format>
#include <iterator>
#include <utility>
#include <vector>

#include "Diagnostic.hpp"
#include "TokenKind.hpp"

namespace milan {

Parser::Parser(std::istream& input) : lexer(input) {}

std::unique_ptr<Program> Parser::parse() {
    getNextToken();
    return parseProgram();
}

Token Parser::getNextToken() {
    return currTok = lexer.nextToken();
}

bool Parser::match(TokenKind expected) {
    if (currTok.is(expected)) {
        getNextToken();
        return true;
    }
    return false;
}

bool Parser::expect(TokenKind expected, std::string_view ctx) {
    if (currTok.isNot(expected)) {
        std::string msg = ctx.empty()
                              ? std::format("Expected '{}', but got {}",
                                            tokenKindToString(expected), formatCurrentToken())
                              : std::format("Expected '{}', but got {} in {}",
                                            tokenKindToString(expected), formatCurrentToken(), ctx);

        Diagnostic::error(currTok.location, msg);
        return false;
    }

    getNextToken();
    return true;
}

std::string Parser::formatCurrentToken() const {
    if (currTok.is(TokenKind::Identifier))
        return std::format("identifier '{}'", currTok.text);
    if (currTok.is(TokenKind::Number))
        return std::format("number '{}'", currTok.numberValue);

    return std::format("'{}'", currTok.name());
}

std::unique_ptr<Program> Parser::parseProgram() {
    if (currTok.isNot(TokenKind::Program))
        return nullptr;

    getNextToken();

    if (currTok.isNot(TokenKind::Identifier))
        return nullptr;

    std::string programName = currTok.text;
    getNextToken();

    if (!expect(TokenKind::Semicolon, "program header"))
        return nullptr;

    std::vector<UPtrDecl> decls;
    while (currTok.is(TokenKind::Const) || currTok.is(TokenKind::Var) ||
           currTok.is(TokenKind::Function) || currTok.is(TokenKind::Procedure)) {
        if (currTok.is(TokenKind::Const)) {
            auto constDecls = parseConstSection();
            std::ranges::move(constDecls, std::back_inserter(decls));
        } else if (currTok.is(TokenKind::Var)) {
            auto varDecls = parseVarSection();
            std::ranges::move(varDecls, std::back_inserter(decls));
        } else if (currTok.is(TokenKind::Function)) {
            auto fnDecl = parseFunction();
            if (!fnDecl)
                return nullptr;
            decls.push_back(std::move(fnDecl));
        } else if (currTok.is(TokenKind::Procedure)) {
            auto procDecl = parseProcedure();
            if (!procDecl)
                return nullptr;
            decls.push_back(std::move(procDecl));
        }
    }

    UPtrStmt mainBlock = parseBlock();
    if (!mainBlock)
        return nullptr;

    if (!expect(TokenKind::Dot, "end of program"))
        return nullptr;

    return std::make_unique<Program>(std::move(programName), std::move(decls),
                                     std::move(mainBlock));
}

} // namespace milan
