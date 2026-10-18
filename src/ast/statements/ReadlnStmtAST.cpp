#include "ReadlnStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>

#include "RealType.hpp"

namespace milan {

ReadlnStmtAST::ReadlnStmtAST(SourceLocation loc, std::string varName)
    : StmtAST(loc), varName(std::move(varName)) {}

llvm::Value* ReadlnStmtAST::codegen(CodegenContext& ctx) {
    const auto* dest = ctx.symbols.lookup(varName);
    if (!dest || !dest->value)
        return nullptr;

    std::string fnName = dest->type.is<RealType>() ? "readlnReal" : "readln";
    llvm::Function* calleeF = ctx.module.getFunction(fnName);
    if (!calleeF)
        return nullptr;

    ctx.builder.CreateCall(calleeF, dest->value);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
