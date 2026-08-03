#include "Program.hpp"

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

    if (mainBlock)
        mainBlock->codegen(ctx);

    ctx.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), 0));
}

} // namespace milan
