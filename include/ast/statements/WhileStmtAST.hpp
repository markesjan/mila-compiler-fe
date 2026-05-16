#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class WhileStmtAST : public StmtAST {
public:
    WhileStmtAST(int row, int col, UPtrExpr cond, UPtrStmt loopStmt);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    UPtrExpr cond;
    UPtrStmt loopStmt;
};

} // namespace milan
