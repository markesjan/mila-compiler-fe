#include "AssignStmtAST.hpp"

#include <utility>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include "ArrayType.hpp"
#include "CodegenContext.hpp"
#include "RealType.hpp"
#include "SymbolTable.hpp"

namespace milan {

AssignStmtAST::AssignStmtAST(SourceLocation loc, std::string name, UPtrExpr val, UPtrExpr idx)
    : StmtAST(loc), name(std::move(name)), val(std::move(val)), idx(std::move(idx)) {}

llvm::Value* AssignStmtAST::codegen(CodegenContext& ctx) {
    if (!val)
        return nullptr;

    llvm::Value* assignedVal = val->codegen(ctx);
    if (!assignedVal)
        return nullptr;

    const auto* dest = ctx.symbols.lookup(name);
    if (!dest || !dest->value)
        return nullptr;

    auto* allocaDest = llvm::dyn_cast_or_null<llvm::AllocaInst>(dest->value);
    if (!allocaDest)
        return nullptr;

    llvm::Value* targetPtr = allocaDest;

    if (idx) {
        llvm::Value* indexVal = idx->codegen(ctx);
        if (!indexVal)
            return nullptr;

        if (dest->type.isArrayType()) {
            int low = dest->type.getLow();
            if (low != 0) {
                llvm::Value* lowVal =
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), low);
                indexVal = ctx.builder.CreateSub(indexVal, lowVal, "subidx");
            }
        }

        llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), 0);
        llvm::Value* indices[] = {zero, indexVal};

        targetPtr =
            ctx.builder.CreateGEP(allocaDest->getAllocatedType(), allocaDest, indices, "arrayidx");
    }

    Type targetType =
        (idx && dest->type.isArrayType()) ? dest->type.as<ArrayType>()->getElemType() : dest->type;

    if (targetType.is<RealType>() && assignedVal->getType()->isIntegerTy()) {
        assignedVal =
            ctx.builder.CreateSIToFP(assignedVal, llvm::Type::getDoubleTy(ctx.context), "conv");
    }

    ctx.builder.CreateStore(assignedVal, targetPtr);
    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
