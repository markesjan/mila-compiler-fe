#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "Token.hpp"

namespace milan {

class UnaryOpExprAST : public ExprAST {
public:
    UnaryOpExprAST(Token op, UPtrExpr expr);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    Token op;
    UPtrExpr expr;
};

} // namespace milan
