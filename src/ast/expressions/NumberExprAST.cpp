#include "NumberExprAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include "CodegenContext.hpp"

namespace milan {

NumberExprAST::NumberExprAST(SourceLocation loc, std::int32_t val) : ExprAST(loc), val(val) {}

llvm::Value* NumberExprAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), val, true);
}

} // namespace milan
