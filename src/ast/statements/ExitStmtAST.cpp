#include "ExitStmtAST.hpp"

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

namespace milan {

ExitStmtAST::ExitStmtAST(int row, int col)
    : StmtAST(row, col) {
}

llvm::Value* ExitStmtAST::codegen(CodegenContext& ctx) {
    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::Type* retType = callerFunc->getReturnType();

    if (retType->isVoidTy()) {
        ctx.builder.CreateRetVoid();
    } else {
        llvm::Value* defaultRet = llvm::Constant::getNullValue(retType);
        ctx.builder.CreateRet(defaultRet);
    }

    llvm::BasicBlock* deadBlock = llvm::BasicBlock::Create(
        ctx.context, "deadcode_after_exit", callerFunc);
    ctx.builder.SetInsertPoint(deadBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
