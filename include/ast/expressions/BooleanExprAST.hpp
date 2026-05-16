#pragma once

#include "ExprAST.hpp"

namespace milan {

class BooleanExprAST : public ExprAST {
public:
    explicit BooleanExprAST(bool val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    bool val;
};

} // namespace milan
