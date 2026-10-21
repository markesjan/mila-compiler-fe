/**
 * @file IfStmtAST.hpp
 * @brief AST node representing conditional branching statements (if-then-else).
 */

#pragma once

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class IfStmtAST : public StmtAST {
public:
    IfStmtAST(SourceLocation loc, UPtrExpr cond, UPtrStmt thenStmt, UPtrStmt elseStmt);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    UPtrExpr cond;
    UPtrStmt thenStmt;
    UPtrStmt elseStmt;
};

} // namespace milan
