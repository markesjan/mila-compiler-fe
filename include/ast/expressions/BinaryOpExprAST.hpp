#pragma once

#include <cstdint>

#include "ExprAST.hpp"

namespace milan {

enum class TokenKind : std::uint8_t;

class BinaryOpExprAST : public ExprAST {
public:
    BinaryOpExprAST(SourceLocation loc, TokenKind op, UPtrExpr lhs, UPtrExpr rhs);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    TokenKind op;
    UPtrExpr lhs, rhs;
};

} // namespace milan
