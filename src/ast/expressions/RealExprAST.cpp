#include "RealExprAST.hpp"

namespace milan {

RealExprAST::RealExprAST(SourceLocation loc, double val) : ExprAST(loc), val(val) {}

llvm::Value* RealExprAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx.context), val);
}

} // namespace milan
