#pragma once

#include "StmtAST.hpp"

namespace milan {

class BreakStmtAST : public StmtAST {
public:
    BreakStmtAST(int row, int col);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
