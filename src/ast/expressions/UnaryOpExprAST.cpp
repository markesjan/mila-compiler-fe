#include "UnaryOpExprAST.hpp"

namespace milan {

UnaryOpExprAST::UnaryOpExprAST(int row, int col, TokenKind op, UPtrExpr expr)
    : ExprAST(row, col), op(op), expr(std::move(expr)) {}

llvm::Value* UnaryOpExprAST::codegen(CodegenContext& ctx) {
    llvm::Value* operandVal = expr->codegen(ctx);
    if (!operandVal)
        return nullptr;

    switch (op) {
        case TokenKind::Minus:
            return ctx.builder.CreateNeg(operandVal, "negtmp");
        case TokenKind::Not:
            return ctx.builder.CreateNot(operandVal, "nottmp");
        default:
            return nullptr;
    }
}

} // namespace milan
