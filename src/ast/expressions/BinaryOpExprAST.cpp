#include "BinaryOpExprAST.hpp"

namespace milan {

BinaryOpExprAST::BinaryOpExprAST(SourceLocation loc, TokenKind op, UPtrExpr lhs, UPtrExpr rhs)
    : ExprAST(loc), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

llvm::Value* BinaryOpExprAST::codegen(CodegenContext& ctx) {
    if (!lhs || !rhs)
        return nullptr;

    llvm::Value* lVal = lhs->codegen(ctx);
    llvm::Value* rVal = rhs->codegen(ctx);
    if (!lVal || !rVal)
        return nullptr;

    bool isFloating =
        op == TokenKind::Slash || lVal->getType()->isDoubleTy() || rVal->getType()->isDoubleTy();

    if (isFloating) {
        if (lVal->getType()->isIntegerTy())
            lVal = ctx.builder.CreateSIToFP(lVal, llvm::Type::getDoubleTy(ctx.context), "convl");
        if (rVal->getType()->isIntegerTy())
            rVal = ctx.builder.CreateSIToFP(rVal, llvm::Type::getDoubleTy(ctx.context), "convr");
    }

    switch (op) {
        // Arithmetical
        case TokenKind::Plus:
            if (isFloating)
                return ctx.builder.CreateFAdd(lVal, rVal, "faddtmp");
            return ctx.builder.CreateAdd(lVal, rVal, "addtmp");
        case TokenKind::Minus:
            if (isFloating)
                return ctx.builder.CreateFSub(lVal, rVal, "fsubtmp");
            return ctx.builder.CreateSub(lVal, rVal, "subtmp");
        case TokenKind::Mul:
            if (isFloating)
                return ctx.builder.CreateFMul(lVal, rVal, "fmultmp");
            return ctx.builder.CreateMul(lVal, rVal, "multmp");
        case TokenKind::Slash:
            return ctx.builder.CreateFDiv(lVal, rVal, "fdivtmp");
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
            if (isFloating)
                return ctx.builder.CreateFCmpOEQ(lVal, rVal, "feqtmp");
            return ctx.builder.CreateICmpEQ(lVal, rVal, "eqtmp");
        case TokenKind::NotEqual:
            if (isFloating)
                return ctx.builder.CreateFCmpONE(lVal, rVal, "fnetmp");
            return ctx.builder.CreateICmpNE(lVal, rVal, "netmp");
        case TokenKind::Less:
            if (isFloating)
                return ctx.builder.CreateFCmpOLT(lVal, rVal, "flttmp");
            return ctx.builder.CreateICmpSLT(lVal, rVal, "lttmp");
        case TokenKind::LessEqual:
            if (isFloating)
                return ctx.builder.CreateFCmpOLE(lVal, rVal, "fletmp");
            return ctx.builder.CreateICmpSLE(lVal, rVal, "letmp");
        case TokenKind::Greater:
            if (isFloating)
                return ctx.builder.CreateFCmpOGT(lVal, rVal, "fgttmp");
            return ctx.builder.CreateICmpSGT(lVal, rVal, "gttmp");
        case TokenKind::GreaterEqual:
            if (isFloating)
                return ctx.builder.CreateFCmpOGE(lVal, rVal, "fgetmp");
            return ctx.builder.CreateICmpSGE(lVal, rVal, "getmp");
        default:
            return nullptr;
    }
}

} // namespace milan
