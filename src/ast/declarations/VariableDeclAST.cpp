#include "VariableDeclAST.hpp"

#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

namespace milan {

VariableDeclAST::VariableDeclAST(int row, int col, std::vector<std::string> names, Type type)
    : DeclAST(row, col), names(std::move(names)), type(type) {
}

llvm::Value* VariableDeclAST::codegen(CodegenContext& ctx) {
    llvm::Type* varType = nullptr;

    switch (type.getBase()) {
        case BaseType::Int:
            varType = llvm::Type::getInt32Ty(ctx.context);
            break;
        case BaseType::Bool:
            varType = llvm::Type::getInt1Ty(ctx.context);
            break;
        default:
            break;
    }

    if (type.isArrayType())
        varType = llvm::ArrayType::get(varType, type.getHigh() - type.getLow() + 1);

    if (!varType)
        return nullptr;

    llvm::Function* callerFunc = ctx.builder.GetInsertBlock()->getParent();

    for (const auto& name : names) {
        llvm::IRBuilder tmpBuilder(&callerFunc->getEntryBlock(),
                                   callerFunc->getEntryBlock().begin());
        llvm::AllocaInst* alloca = tmpBuilder.CreateAlloca(varType, nullptr, name.c_str());
        ctx.symbolTable[name] = alloca;
    }

    return nullptr;
}

} // namespace milan
