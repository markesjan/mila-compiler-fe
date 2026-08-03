#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "ExprAST.hpp"
#include "Lexer.hpp"
#include "Program.hpp"
#include "StmtAST.hpp"

namespace milan {

class Parser {
public:
    Parser();
    ~Parser() = default;

    bool parse();
    const llvm::Module& generate();

private:
    Lexer lexer;
    int currTok;
    std::unique_ptr<Program> programAST;

    llvm::LLVMContext MilaContext;
    llvm::IRBuilder<> MilaBuilder;
    llvm::Module MilaModule;

    int getNextToken();
    bool match(int expectedToken);

    bool parseProgram();
    UPtrStmt parseBlock();
    std::vector<UPtrStmt> parseStatements();
    UPtrExpr parseExpression();
    UPtrExpr parseSimpleExpr();
    UPtrExpr parseTerm();
    UPtrExpr parseFactor();
    UPtrExpr parseFactorIdentifier(int row, int col);
};

} // namespace milan
