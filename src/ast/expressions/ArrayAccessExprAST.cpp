#include "ArrayAccessExprAST.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

namespace milan {

ArrayAccessExprAST::ArrayAccessExprAST(int row, int col, std::string name, UPtrExpr idx)
    : ExprAST(row, col), name(std::move(name)), idx(std::move(idx)) {}

llvm::Value* ArrayAccessExprAST::codegen(CodegenContext& ctx) {
    llvm::Value* arrayVal = ctx.symbolTable[name];
    if (!arrayVal)
        return nullptr;

    auto* alloca = llvm::dyn_cast<llvm::AllocaInst>(arrayVal);
    if (!alloca)
        return nullptr;

    llvm::Value* indexVal = idx->codegen(ctx);
    if (!indexVal)
        return nullptr;

    auto it = ctx.typeTable.find(name);
    if (it != ctx.typeTable.end() && it->second.isArrayType()) {
        int low = it->second.getLow();
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
