#pragma once

#include "StmtAST.hpp"

namespace milan {

class BreakStmtAST : public StmtAST {
public:
    explicit BreakStmtAST(SourceLocation loc);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
