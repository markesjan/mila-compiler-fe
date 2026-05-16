#pragma once

#include "StmtAST.hpp"

namespace milan {

class BreakStmtAST : public StmtAST {
public:
    explicit BreakStmtAST();
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
