#include "ContinueStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>

namespace milan {

ContinueStmtAST::ContinueStmtAST(int row, int col)
    : StmtAST(row, col) {
}

llvm::Value* ContinueStmtAST::codegen(CodegenContext& ctx) {
    if (ctx.loopContinueBlocks.empty())
        return nullptr;

    llvm::BasicBlock* continueTarget = ctx.loopContinueBlocks.back();
    ctx.builder.CreateBr(continueTarget);

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* deadBlock = llvm::BasicBlock::Create(
        ctx.context, "deadcode_after_continue", callerFunc);
    ctx.builder.SetInsertPoint(deadBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
