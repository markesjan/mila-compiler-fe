#pragma once

#include <memory>

#include "ExprAST.hpp"

namespace milan {

class UnOpExprAST : public ExprAST {
public:
    UnOpExprAST(char op, std::unique_ptr<ExprAST> expr);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    char op;
    std::unique_ptr<ExprAST> expr;
};

} // namespace milan
