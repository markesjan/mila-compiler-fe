#pragma once

#include "StmtAST.hpp"

namespace milan {

class EmptyStmtAST : public StmtAST {
public:
    EmptyStmtAST(int row, int col);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
