#pragma once

#include <memory>

#include "ExprAST.hpp"

namespace milan {

class BinOpExprAST : public ExprAST {
public:
    BinOpExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    char op;
    std::unique_ptr<ExprAST> lhs, rhs;
};

} // namespace milan
