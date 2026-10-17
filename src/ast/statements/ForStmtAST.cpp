#include "ForStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

namespace milan {

ForStmtAST::ForStmtAST(SourceLocation loc,
                       std::string name,
                       UPtrExpr start,
                       UPtrExpr stop,
                       TokenKind direction,
                       UPtrStmt loopStmt)
    : StmtAST(loc),
      name(std::move(name)),
      start(std::move(start)),
      stop(std::move(stop)),
      direction(direction),
      loopStmt(std::move(loopStmt)) {}

llvm::Value* ForStmtAST::codegen(CodegenContext& ctx) {
    llvm::Value* varAllocaRaw = ctx.symbolTable[name];
    llvm::AllocaInst* varAlloca = llvm::dyn_cast_or_null<llvm::AllocaInst>(varAllocaRaw);
    if (!varAlloca)
        return nullptr;

    llvm::Value* startVal = start->codegen(ctx);
    if (!startVal)
        return nullptr;
    ctx.builder.CreateStore(startVal, varAlloca);

    llvm::Value* stopVal = stop->codegen(ctx);
    if (!stopVal)
        return nullptr;

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* condBlock = llvm::BasicBlock::Create(ctx.context, "forcond", callerFunc);
    llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(ctx.context, "forloop");
    llvm::BasicBlock* afterBlock = llvm::BasicBlock::Create(ctx.context, "forcont");

    ctx.builder.CreateBr(condBlock);
    ctx.builder.SetInsertPoint(condBlock);

    llvm::Value* currVal =
        ctx.builder.CreateLoad(varAlloca->getAllocatedType(), varAlloca, name.c_str());
    llvm::Value* condResult = nullptr;
    if (direction == TokenKind::To)
        condResult = ctx.builder.CreateICmpSLE(currVal, stopVal, "tocond");
    else
        condResult = ctx.builder.CreateICmpSGE(currVal, stopVal, "downtocond");

    ctx.builder.CreateCondBr(condResult, loopBlock, afterBlock);

    llvm::BasicBlock* stepBlock = llvm::BasicBlock::Create(ctx.context, "forstep");

    callerFunc->insert(callerFunc->end(), loopBlock);
    ctx.builder.SetInsertPoint(loopBlock);

    ctx.loopBreakBlocks.push_back(afterBlock);
    ctx.loopContinueBlocks.push_back(stepBlock);

    if (!loopStmt->codegen(ctx)) {
        ctx.loopBreakBlocks.pop_back();
        ctx.loopContinueBlocks.pop_back();
        return nullptr;
    }

    ctx.loopBreakBlocks.pop_back();
    ctx.loopContinueBlocks.pop_back();

    ctx.builder.CreateBr(stepBlock);

    callerFunc->insert(callerFunc->end(), stepBlock);
    ctx.builder.SetInsertPoint(stepBlock);

    llvm::Value* stepLoad =
        ctx.builder.CreateLoad(varAlloca->getAllocatedType(), varAlloca, "stepload");
    llvm::Value* one = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), 1);
    llvm::Value* nextVal = nullptr;

    if (direction == TokenKind::To)
        nextVal = ctx.builder.CreateAdd(stepLoad, one, "nextval");
    else
        nextVal = ctx.builder.CreateSub(stepLoad, one, "nextval");

    ctx.builder.CreateStore(nextVal, varAlloca);
    ctx.builder.CreateBr(condBlock);
    callerFunc->insert(callerFunc->end(), afterBlock);
    ctx.builder.SetInsertPoint(afterBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
