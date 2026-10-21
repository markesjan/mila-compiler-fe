#include "RealExprAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include "CodegenContext.hpp"

namespace milan {

RealExprAST::RealExprAST(SourceLocation loc, double val) : ExprAST(loc), val(val) {}

llvm::Value* RealExprAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx.context), val);
}

} // namespace milan
