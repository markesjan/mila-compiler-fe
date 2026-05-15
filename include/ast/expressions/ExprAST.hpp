#pragma once

#include "CodegenContext.hpp"

namespace milan {

class ExprAST {
public:
    ExprAST(int row, int col);
    virtual ~ExprAST() = default;
    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;

private:
    int row, col;
};

} // namespace milan
