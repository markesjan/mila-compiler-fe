#include "BooleanExprAST.hpp"

namespace milan {

BooleanExprAST::BooleanExprAST(int row, int col, bool val)
    : ExprAST(row, col), val(val) {
}

llvm::Value* BooleanExprAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx.context), val);
}

} // namespace milan
