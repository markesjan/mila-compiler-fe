#include "VariableExprAST.hpp"

#include <utility>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>

#include "CodegenContext.hpp"
#include "SymbolTable.hpp"

namespace milan {

VariableExprAST::VariableExprAST(SourceLocation loc, std::string name)
    : ExprAST(loc), name(std::move(name)) {}

llvm::Value* VariableExprAST::codegen(CodegenContext& ctx) {
    const auto* var = ctx.symbols.lookup(name);
    if (!var || !var->value)
        return nullptr;

    auto* alloca = llvm::dyn_cast<llvm::AllocaInst>(var->value);
    if (alloca)
        return ctx.builder.CreateLoad(alloca->getAllocatedType(), alloca, name.c_str());

    return var->value;
}

} // namespace milan
