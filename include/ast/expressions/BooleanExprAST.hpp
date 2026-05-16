#pragma once

#include "ExprAST.hpp"

namespace milan {

class BooleanExprAST : public ExprAST {
public:
    BooleanExprAST(int row, int col, bool val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    bool val;
};

} // namespace milan
