#include "UnaryOpExprAST.hpp"

namespace milan {

UnaryOpExprAST::UnaryOpExprAST(SourceLocation loc, TokenKind op, UPtrExpr expr)
    : ExprAST(loc), op(op), expr(std::move(expr)) {}

llvm::Value* UnaryOpExprAST::codegen(CodegenContext& ctx) {
    if (!expr)
        return nullptr;

    llvm::Value* operandVal = expr->codegen(ctx);
    if (!operandVal)
        return nullptr;

    switch (op) {
        case TokenKind::Minus:
            if (operandVal->getType()->isDoubleTy())
                return ctx.builder.CreateFNeg(operandVal, "fnegtmp");
            return ctx.builder.CreateNeg(operandVal, "negtmp");
        case TokenKind::Not:
            return ctx.builder.CreateNot(operandVal, "nottmp");
        default:
            return nullptr;
    }
}

} // namespace milan
