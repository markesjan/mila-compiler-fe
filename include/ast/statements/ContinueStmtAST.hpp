#pragma once

#include "StmtAST.hpp"

namespace milan {

class ContinueStmtAST : public StmtAST {
public:
    explicit ContinueStmtAST();
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
