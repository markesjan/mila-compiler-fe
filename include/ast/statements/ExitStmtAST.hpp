#pragma once

#include "StmtAST.hpp"

namespace milan {

class ExitStmtAST : public StmtAST {
public:
    ExitStmtAST(int row, int col);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
