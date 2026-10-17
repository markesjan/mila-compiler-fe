#include "ConstantDeclAST.hpp"

namespace milan {

ConstantDeclAST::ConstantDeclAST(SourceLocation loc, std::string name, UPtrExpr val)
    : DeclAST(loc), name(std::move(name)), val(std::move(val)) {}

llvm::Value* ConstantDeclAST::codegen(CodegenContext& ctx) {
    llvm::Value* constVal = val->codegen(ctx);
    if (!constVal)
        return nullptr;

    BaseType baseType = constVal->getType()->isIntegerTy(1) ? BaseType::Bool : BaseType::Int;
    ctx.symbols.insert(name, Type(baseType), constVal, true);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
