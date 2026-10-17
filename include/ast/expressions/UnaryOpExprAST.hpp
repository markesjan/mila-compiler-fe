#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "TokenKind.hpp"

namespace milan {

class UnaryOpExprAST : public ExprAST {
public:
    UnaryOpExprAST(SourceLocation loc, TokenKind op, UPtrExpr expr);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    TokenKind op;
    UPtrExpr expr;
};

} // namespace milan
