#pragma once

#include "ExprAST.hpp"

namespace milan {

class CallExprAST : public ExprAST {
public:
    CallExprAST();
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
