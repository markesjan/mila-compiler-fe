#pragma once

#include <cstdint>

#include "ExprAST.hpp"

namespace milan {

enum class TokenKind : std::uint8_t;

class UnaryOpExprAST : public ExprAST {
public:
    UnaryOpExprAST(SourceLocation loc, TokenKind op, UPtrExpr expr);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    TokenKind op;
    UPtrExpr expr;
};

} // namespace milan
