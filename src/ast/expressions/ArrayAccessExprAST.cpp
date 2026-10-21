#include "ArrayAccessExprAST.hpp"

#include <utility>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

#include "CodegenContext.hpp"
#include "SymbolTable.hpp"

namespace milan {

ArrayAccessExprAST::ArrayAccessExprAST(SourceLocation loc, std::string name, UPtrExpr idx)
    : ExprAST(loc), name(std::move(name)), idx(std::move(idx)) {}

llvm::Value* ArrayAccessExprAST::codegen(CodegenContext& ctx) {
    const auto* arraySym = ctx.symbols.lookup(name);
    if (!arraySym || !arraySym->value)
        return nullptr;

    auto* alloca = llvm::dyn_cast<llvm::AllocaInst>(arraySym->value);
    if (!alloca)
        return nullptr;

    if (!idx)
        return nullptr;

    llvm::Value* indexVal = idx->codegen(ctx);
    if (!indexVal)
        return nullptr;

    if (arraySym->type.isArrayType()) {
        int low = arraySym->type.getLow();
        if (low != 0) {
            llvm::Value* lowVal = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), low);
            indexVal = ctx.builder.CreateSub(indexVal, lowVal, "subidx");
        }
    }

    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), 0);
    llvm::Value* indices[] = {zero, indexVal};

    llvm::Value* elemPtr =
        ctx.builder.CreateGEP(alloca->getAllocatedType(), alloca, indices, "arrayidx");

    return ctx.builder.CreateLoad(alloca->getAllocatedType()->getArrayElementType(), elemPtr,
                                  "arrayload");
}

} // namespace milan
