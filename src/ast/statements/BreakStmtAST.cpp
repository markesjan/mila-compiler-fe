#include "BreakStmtAST.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "CodegenContext.hpp"
#include "LoopStack.hpp"

namespace milan {

BreakStmtAST::BreakStmtAST(SourceLocation loc) : StmtAST(loc) {}

llvm::Value* BreakStmtAST::codegen(CodegenContext& ctx) {
    if (ctx.loops.empty())
        return nullptr;

    llvm::BasicBlock* breakTarget = ctx.loops.getBreakTarget();
    ctx.builder.CreateBr(breakTarget);

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* deadBlock =
        llvm::BasicBlock::Create(ctx.context, "deadcode_after_break", callerFunc);
    ctx.builder.SetInsertPoint(deadBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
