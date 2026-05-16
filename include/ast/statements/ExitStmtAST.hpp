#pragma once

#include "StmtAST.hpp"

namespace milan {

class ExitStmtAST : public StmtAST {
public:
    explicit ExitStmtAST();
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
