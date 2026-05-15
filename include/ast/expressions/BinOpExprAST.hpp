#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "Token.hpp"

namespace milan {

class BinOpExprAST : public ExprAST {
public:
    BinOpExprAST(Token op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    Token op;
    std::unique_ptr<ExprAST> lhs, rhs;
};

} // namespace milan
