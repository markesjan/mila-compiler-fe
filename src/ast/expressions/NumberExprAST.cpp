#include "NumberExprAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

namespace milan {

NumberExprAST::NumberExprAST(int row, int col, std::int32_t val)
    : ExprAST(row, col), val(val) {
}

llvm::Value* NumberExprAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), val, true);
}

} // namespace milan
