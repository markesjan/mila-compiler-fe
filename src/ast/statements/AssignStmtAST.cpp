#include "AssignStmtAST.hpp"

namespace milan {

AssignStmtAST::AssignStmtAST(int row, int col, std::string name, UPtrExpr val, UPtrExpr idx)
    : StmtAST(row, col), name(std::move(name)), val(std::move(val)), idx(std::move(idx)) {}

llvm::Value* AssignStmtAST::codegen(CodegenContext& ctx) {
    llvm::Value* assignedVal = val->codegen(ctx);
    if (!assignedVal)
        return nullptr;

    llvm::Value* dest = ctx.symbolTable[name];
    llvm::AllocaInst* allocaDest = llvm::dyn_cast_or_null<llvm::AllocaInst>(dest);

    if (!allocaDest)
        return nullptr;

    llvm::Value* targetPtr = allocaDest;

    if (idx) {
        llvm::Value* indexVal = idx->codegen(ctx);
        if (!indexVal)
            return nullptr;

        llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx.context), 0);
        llvm::Value* indices[] = {zero, indexVal};

        targetPtr =
            ctx.builder.CreateGEP(allocaDest->getAllocatedType(), allocaDest, indices, "arrayidx");
    }

    ctx.builder.CreateStore(assignedVal, targetPtr);
    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
