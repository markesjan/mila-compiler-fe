#pragma once

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class WhileStmtAST : public StmtAST {
public:
    WhileStmtAST(SourceLocation loc, UPtrExpr cond, UPtrStmt loopStmt);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    UPtrExpr cond;
    UPtrStmt loopStmt;
};

} // namespace milan
