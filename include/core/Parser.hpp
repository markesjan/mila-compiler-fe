#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "DeclAST.hpp"
#include "ExprAST.hpp"
#include "Lexer.hpp"
#include "Program.hpp"
#include "StmtAST.hpp"
#include "Token.hpp"
#include "Type.hpp"

namespace milan {

class Parser {
public:
    explicit Parser(std::istream& input = std::cin);
    ~Parser() = default;

    std::unique_ptr<Program> parse();

private:
    Lexer lexer;
    Token currTok;

    Token getNextToken();
    bool match(TokenKind expected);
    bool expect(TokenKind expected, std::string_view ctx = "");
    std::string formatCurrentToken() const;

    std::unique_ptr<Program> parseProgram();
    std::optional<Type> parseType();

    std::vector<UPtrDecl> parseConstSection();
    std::vector<UPtrDecl> parseVarSection();
    std::vector<UPtrDecl> parseParameters();
    UPtrDecl parseFunction();
    UPtrDecl parseProcedure();

    UPtrStmt parseBlock();
    std::vector<UPtrStmt> parseStatements();
    UPtrStmt parseStatement();
    UPtrStmt parseIfStatement();
    UPtrStmt parseWhileStatement();
    UPtrStmt parseForStatement();

    UPtrExpr parseExpression();
    UPtrExpr parseSimpleExpr();
    UPtrExpr parseTerm();
    UPtrExpr parseFactor();
    UPtrExpr parseFactorIdentifier(SourceLocation loc);
};

} // namespace milan
