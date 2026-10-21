#include "Program.hpp"

#include <memory>
#include <utility>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

#include "CodegenContext.hpp"

namespace milan {

Program::Program(std::string name, std::vector<UPtrDecl> globalDecls, UPtrStmt mainBlock)
    : name(std::move(name)), globalDecls(std::move(globalDecls)), mainBlock(std::move(mainBlock)) {}

void Program::codegen(CodegenContext& ctx) {
    llvm::FunctionType* funcType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx.context), false);
    llvm::Function* mainFunc =
        llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", ctx.module);

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(ctx.context, "entry", mainFunc);
    ctx.builder.SetInsertPoint(entryBlock);

    for (const auto& d : globalDecls) {
        if (d)
            d->codegen(ctx);
    }

    if (mainBlock)
        mainBlock->codegen(ctx);

    if (!ctx.builder.GetInsertBlock()->getTerminator())
        ctx.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), 0));
}

} // namespace milan
