#include "ConstantDeclAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>

#include <utility>

#include "CodegenContext.hpp"
#include "Type.hpp"

namespace milan {

ConstantDeclAST::ConstantDeclAST(SourceLocation loc, std::string name, UPtrExpr val)
    : DeclAST(loc), name(std::move(name)), val(std::move(val)) {}

llvm::Value* ConstantDeclAST::codegen(CodegenContext& ctx) {
    if (!val)
        return nullptr;

    llvm::Value* constVal = val->codegen(ctx);
    if (!constVal)
        return nullptr;

    Type type = Type::fromLlvmType(constVal->getType());
    ctx.symbols.insert(name, type, constVal, true);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
