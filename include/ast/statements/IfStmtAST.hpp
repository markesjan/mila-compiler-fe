#pragma once

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class IfStmtAST : public StmtAST {
public:
    IfStmtAST(int row, int col, UPtrExpr cond, UPtrStmt thenStmt, UPtrStmt elseStmt);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    UPtrExpr cond;
    UPtrStmt thenStmt;
    UPtrStmt elseStmt;
};

} // namespace milan
