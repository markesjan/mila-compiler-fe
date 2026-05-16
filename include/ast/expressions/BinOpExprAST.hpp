#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "Token.hpp"

namespace milan {

class BinOpExprAST : public ExprAST {
public:
    BinOpExprAST(Token op, UPtrExpr lhs, UPtrExpr rhs);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    Token op;
    UPtrExpr lhs, rhs;
};

} // namespace milan
