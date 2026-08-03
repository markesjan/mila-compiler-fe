#include "BinaryOpExprAST.hpp"

namespace milan {

BinaryOpExprAST::BinaryOpExprAST(int row, int col, Token op, UPtrExpr lhs, UPtrExpr rhs)
    : ExprAST(row, col), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

llvm::Value* BinaryOpExprAST::codegen(CodegenContext& ctx) {
    llvm::Value* lVal = lhs->codegen(ctx);
    llvm::Value* rVal = rhs->codegen(ctx);
    if (!lVal || !rVal)
        return nullptr;

    switch (op) {
        // Arithmetical
        case '+':
            return ctx.builder.CreateAdd(lVal, rVal, "addtmp");
        case '-':
            return ctx.builder.CreateSub(lVal, rVal, "subtmp");
        case '*':
            return ctx.builder.CreateMul(lVal, rVal, "multmp");
        case tok_div:
            return ctx.builder.CreateSDiv(lVal, rVal, "divtmp");
        case tok_mod:
            return ctx.builder.CreateSRem(lVal, rVal, "modtmp");
        // Logical
        case tok_and:
            return ctx.builder.CreateAnd(lVal, rVal, "andtmp");
        case tok_or:
            return ctx.builder.CreateOr(lVal, rVal, "ortmp");
        case tok_xor:
            return ctx.builder.CreateXor(lVal, rVal, "xortmp");
        // Relational
        case '=':
            return ctx.builder.CreateICmpEQ(lVal, rVal, "eqtmp");
        case tok_notequal:
            return ctx.builder.CreateICmpNE(lVal, rVal, "netmp");
        case '<':
            return ctx.builder.CreateICmpSLT(lVal, rVal, "lttmp");
        case tok_lessequal:
            return ctx.builder.CreateICmpSLE(lVal, rVal, "letmp");
        case '>':
            return ctx.builder.CreateICmpSGT(lVal, rVal, "gttmp");
        case tok_greaterequal:
            return ctx.builder.CreateICmpSGE(lVal, rVal, "getmp");
        default:
            return nullptr;
    }
}

} // namespace milan
