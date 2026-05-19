#include "BreakStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>

namespace milan {

BreakStmtAST::BreakStmtAST(int row, int col)
    : StmtAST(row, col) {
}

llvm::Value* BreakStmtAST::codegen(CodegenContext& ctx) {
    if (ctx.loopBreakBlocks.empty())
        return nullptr;

    llvm::BasicBlock* breakTarget = ctx.loopBreakBlocks.back();
    ctx.builder.CreateBr(breakTarget);

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* deadBlock = llvm::BasicBlock::Create(
        ctx.context, "deadcode_after_break", callerFunc);
    ctx.builder.SetInsertPoint(deadBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
