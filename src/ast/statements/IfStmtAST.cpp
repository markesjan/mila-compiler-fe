#include "IfStmtAST.hpp"

#include <utility>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>

#include "CodegenContext.hpp"

namespace milan {

IfStmtAST::IfStmtAST(SourceLocation loc, UPtrExpr cond, UPtrStmt thenStmt, UPtrStmt elseStmt)
    : StmtAST(loc),
      cond(std::move(cond)),
      thenStmt(std::move(thenStmt)),
      elseStmt(std::move(elseStmt)) {}

llvm::Value* IfStmtAST::codegen(CodegenContext& ctx) {
    if (!cond)
        return nullptr;

    llvm::Value* condVal = cond->codegen(ctx);
    if (!condVal)
        return nullptr;

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(ctx.context, "then", callerFunc);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(ctx.context, "else");
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(ctx.context, "ifcont");

    ctx.builder.CreateCondBr(condVal, thenBlock, elseBlock);

    ctx.builder.SetInsertPoint(thenBlock);
    if (thenStmt && !thenStmt->codegen(ctx))
        return nullptr;
    ctx.builder.CreateBr(mergeBlock);

    callerFunc->insert(callerFunc->end(), elseBlock);
    ctx.builder.SetInsertPoint(elseBlock);
    if (elseStmt && !elseStmt->codegen(ctx))
        return nullptr;
    ctx.builder.CreateBr(mergeBlock);

    callerFunc->insert(callerFunc->end(), mergeBlock);
    ctx.builder.SetInsertPoint(mergeBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
