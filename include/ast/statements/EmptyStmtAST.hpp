#pragma once

#include "StmtAST.hpp"

namespace milan {

class EmptyStmtAST : public StmtAST {
public:
    explicit EmptyStmtAST();
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
