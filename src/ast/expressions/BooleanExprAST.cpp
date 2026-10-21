#include "BooleanExprAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include "CodegenContext.hpp"

namespace milan {

BooleanExprAST::BooleanExprAST(SourceLocation loc, bool val) : ExprAST(loc), val(val) {}

llvm::Value* BooleanExprAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx.context), val);
}

} // namespace milan
