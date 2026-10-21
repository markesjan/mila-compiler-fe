#include "ContinueStmtAST.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "CodegenContext.hpp"
#include "LoopStack.hpp"

namespace milan {

ContinueStmtAST::ContinueStmtAST(SourceLocation loc) : StmtAST(loc) {}

llvm::Value* ContinueStmtAST::codegen(CodegenContext& ctx) {
    if (ctx.loops.empty())
        return nullptr;

    llvm::BasicBlock* continueTarget = ctx.loops.getContinueTarget();
    ctx.builder.CreateBr(continueTarget);

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* deadBlock =
        llvm::BasicBlock::Create(ctx.context, "deadcode_after_continue", callerFunc);
    ctx.builder.SetInsertPoint(deadBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
