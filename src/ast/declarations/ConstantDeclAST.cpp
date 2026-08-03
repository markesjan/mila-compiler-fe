#include "ConstantDeclAST.hpp"

namespace milan {

ConstantDeclAST::ConstantDeclAST(int row, int col, std::string name, UPtrExpr val)
    : DeclAST(row, col), name(std::move(name)), val(std::move(val)) {}

llvm::Value* ConstantDeclAST::codegen(CodegenContext& ctx) {
    llvm::Value* constVal = val->codegen(ctx);
    if (!constVal)
        return nullptr;

    ctx.symbolTable[name] = constVal;

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
