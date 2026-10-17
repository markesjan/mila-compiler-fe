#include "ReadlnStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>

namespace milan {

ReadlnStmtAST::ReadlnStmtAST(SourceLocation loc, std::string varName)
    : StmtAST(loc), varName(std::move(varName)) {}

llvm::Value* ReadlnStmtAST::codegen(CodegenContext& ctx) {
    llvm::Function* calleeF = ctx.module.getFunction("readln");
    if (!calleeF)
        return nullptr;

    llvm::Value* dest = ctx.symbolTable[varName];
    if (!dest)
        return nullptr;

    ctx.builder.CreateCall(calleeF, dest);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
