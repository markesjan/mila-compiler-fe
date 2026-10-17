#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "Diagnostic.hpp"
#include "ExprAST.hpp"
#include "Lexer.hpp"
#include "Program.hpp"
#include "StmtAST.hpp"
#include "Type.hpp"

namespace milan {

class Parser {
public:
    Parser();
    ~Parser() = default;

    bool parse();
    const llvm::Module& generate();

private:
    Lexer lexer;
    Token currTok;
    std::unique_ptr<Program> programAST;

    llvm::LLVMContext MilaContext;
    llvm::IRBuilder<> MilaBuilder;
    llvm::Module MilaModule;

    Token getNextToken();
    bool match(TokenKind expected);
    bool expect(TokenKind expected, std::string_view ctx = "");
    std::string formatCurrentToken() const;

    bool parseProgram();
    std::optional<Type> parseType();

    std::vector<UPtrDecl> parseConstSection();
    std::vector<UPtrDecl> parseVarSection();

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
