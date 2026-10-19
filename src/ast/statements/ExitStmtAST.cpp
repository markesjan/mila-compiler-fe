#include "ExitStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

namespace milan {

ExitStmtAST::ExitStmtAST(SourceLocation loc) : StmtAST(loc) {}

llvm::Value* ExitStmtAST::codegen(CodegenContext& ctx) {
    auto* insertBlock = ctx.builder.GetInsertBlock();
    if (!insertBlock)
        return nullptr;

    llvm::Function* callerFunc = insertBlock->getParent();
    if (!callerFunc)
        return nullptr;

    llvm::Type* retType = callerFunc->getReturnType();

    if (retType->isVoidTy()) {
        ctx.builder.CreateRetVoid();
    } else {
        llvm::Value* retVal = nullptr;

        if (auto* retSym = ctx.symbols.lookup(callerFunc->getName()))
            retVal = ctx.builder.CreateLoad(retType, retSym->value, "exitVal");
        else
            retVal = llvm::Constant::getNullValue(retType);

        ctx.builder.CreateRet(retVal);
    }

    llvm::BasicBlock* deadBlock =
        llvm::BasicBlock::Create(ctx.context, "deadcode_after_exit", callerFunc);
    ctx.builder.SetInsertPoint(deadBlock);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
