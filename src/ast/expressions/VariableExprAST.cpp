#include "VariableExprAST.hpp"

#include <llvm/IR/Instructions.h>

namespace milan {

VariableExprAST::VariableExprAST(SourceLocation loc, std::string name)
    : ExprAST(loc), name(std::move(name)) {}

llvm::Value* VariableExprAST::codegen(CodegenContext& ctx) {
    llvm::Value* v = ctx.symbolTable[name];
    if (!v)
        return nullptr;

    auto* alloca = llvm::dyn_cast<llvm::AllocaInst>(v);
    if (alloca)
        return ctx.builder.CreateLoad(alloca->getAllocatedType(), alloca, name.c_str());

    return v;
}

} // namespace milan
