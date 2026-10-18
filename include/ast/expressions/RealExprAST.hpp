#pragma once

#include "ExprAST.hpp"

namespace milan {

class RealExprAST : public ExprAST {
public:
    RealExprAST(SourceLocation loc, double val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    double val;
};

} // namespace milan
