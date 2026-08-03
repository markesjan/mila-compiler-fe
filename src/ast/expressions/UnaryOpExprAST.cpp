#include "UnaryOpExprAST.hpp"

namespace milan {

UnaryOpExprAST::UnaryOpExprAST(int row, int col, Token op, UPtrExpr expr)
    : ExprAST(row, col), op(op), expr(std::move(expr)) {}

llvm::Value* UnaryOpExprAST::codegen(CodegenContext& ctx) {
    llvm::Value* operandVal = expr->codegen(ctx);
    if (!operandVal)
        return nullptr;

    switch (op) {
        case '-':
            return ctx.builder.CreateNeg(operandVal, "negtmp");
        case tok_not:
            return ctx.builder.CreateNot(operandVal, "nottmp");
        default:
            return nullptr;
    }
}

} // namespace milan
