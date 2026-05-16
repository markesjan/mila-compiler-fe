#pragma once

#include "StmtAST.hpp"

namespace milan {

class ContinueStmtAST : public StmtAST {
public:
    ContinueStmtAST(int row, int col);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
