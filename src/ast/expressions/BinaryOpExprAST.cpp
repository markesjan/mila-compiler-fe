#include "BinaryOpExprAST.hpp"

namespace milan {

BinaryOpExprAST::BinaryOpExprAST(SourceLocation loc, TokenKind op, UPtrExpr lhs, UPtrExpr rhs)
    : ExprAST(loc), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

llvm::Value* BinaryOpExprAST::codegen(CodegenContext& ctx) {
    llvm::Value* lVal = lhs->codegen(ctx);
    llvm::Value* rVal = rhs->codegen(ctx);
    if (!lVal || !rVal)
        return nullptr;

    switch (op) {
        // Arithmetical
        case TokenKind::Plus:
            return ctx.builder.CreateAdd(lVal, rVal, "addtmp");
        case TokenKind::Minus:
            return ctx.builder.CreateSub(lVal, rVal, "subtmp");
        case TokenKind::Mul:
            return ctx.builder.CreateMul(lVal, rVal, "multmp");
        case TokenKind::Div:
            return ctx.builder.CreateSDiv(lVal, rVal, "divtmp");
        case TokenKind::Mod:
            return ctx.builder.CreateSRem(lVal, rVal, "modtmp");
        // Logical
        case TokenKind::And:
            return ctx.builder.CreateAnd(lVal, rVal, "andtmp");
        case TokenKind::Or:
            return ctx.builder.CreateOr(lVal, rVal, "ortmp");
        case TokenKind::Xor:
            return ctx.builder.CreateXor(lVal, rVal, "xortmp");
        // Relational
        case TokenKind::Equal:
            return ctx.builder.CreateICmpEQ(lVal, rVal, "eqtmp");
        case TokenKind::NotEqual:
            return ctx.builder.CreateICmpNE(lVal, rVal, "netmp");
        case TokenKind::Less:
            return ctx.builder.CreateICmpSLT(lVal, rVal, "lttmp");
        case TokenKind::LessEqual:
            return ctx.builder.CreateICmpSLE(lVal, rVal, "letmp");
        case TokenKind::Greater:
            return ctx.builder.CreateICmpSGT(lVal, rVal, "gttmp");
        case TokenKind::GreaterEqual:
            return ctx.builder.CreateICmpSGE(lVal, rVal, "getmp");
        default:
            return nullptr;
    }
}

} // namespace milan
