#include "VariableDeclAST.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>

#include <utility>

#include "CodegenContext.hpp"
#include "SymbolTable.hpp"

namespace milan {

VariableDeclAST::VariableDeclAST(SourceLocation loc, std::vector<std::string> names, Type type)
    : DeclAST(loc), names(std::move(names)), type(std::move(type)) {}

llvm::Value* VariableDeclAST::codegen(CodegenContext& ctx) {
    llvm::Type* varType = type.toLlvmType(ctx.context);
    if (!varType)
        return nullptr;

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();

    for (const auto& name : names) {
        llvm::IRBuilder tmpBuilder(&callerFunc->getEntryBlock(),
                                   callerFunc->getEntryBlock().begin());
        llvm::AllocaInst* alloca = tmpBuilder.CreateAlloca(varType, nullptr, name);
        ctx.symbols.insert(name, type, alloca);
    }

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
