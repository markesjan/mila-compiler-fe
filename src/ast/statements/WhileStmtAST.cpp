#include "WhileStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>

#include "LoopGuard.hpp"

namespace milan {

WhileStmtAST::WhileStmtAST(SourceLocation loc, UPtrExpr cond, UPtrStmt loopStmt)
    : StmtAST(loc), cond(std::move(cond)), loopStmt(std::move(loopStmt)) {}

llvm::Value* WhileStmtAST::codegen(CodegenContext& ctx) {
    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* condBlock = llvm::BasicBlock::Create(ctx.context, "whilecond", callerFunc);
    llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(ctx.context, "whileloop");
    llvm::BasicBlock* afterBlock = llvm::BasicBlock::Create(ctx.context, "whilecont");

    ctx.builder.CreateBr(condBlock);
    ctx.builder.SetInsertPoint(condBlock);

    if (!cond)
        return nullptr;

    llvm::Value* condVal = cond->codegen(ctx);
    if (!condVal)
        return nullptr;

    ctx.builder.CreateCondBr(condVal, loopBlock, afterBlock);
    callerFunc->insert(callerFunc->end(), loopBlock);
    ctx.builder.SetInsertPoint(loopBlock);

    LoopGuard loopGuard(ctx.loops, afterBlock, condBlock);
    if (loopStmt && !loopStmt->codegen(ctx))
        return nullptr;

    ctx.builder.CreateBr(condBlock);
    callerFunc->insert(callerFunc->end(), afterBlock);
    ctx.builder.SetInsertPoint(afterBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
