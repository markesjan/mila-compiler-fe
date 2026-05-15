#pragma once

#include "ExprAST.hpp"

namespace milan {

class BoolExprAST : public ExprAST {
public:
    explicit BoolExprAST(bool val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    bool val;
};

} // namespace milan
