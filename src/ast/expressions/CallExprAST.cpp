#include "CallExprAST.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

namespace milan {

CallExprAST::CallExprAST(SourceLocation loc, std::string name, std::vector<UPtrExpr> args)
    : ExprAST(loc), name(std::move(name)), args(std::move(args)) {}

llvm::Value* CallExprAST::codegen(CodegenContext& ctx) {
    std::vector<llvm::Value*> argsV;
    for (const auto& arg : args) {
        if (!arg)
            return nullptr;
        argsV.push_back(arg->codegen(ctx));
        if (!argsV.back())
            return nullptr;
    }

    std::string fnName = name;
    if ((name == "writeln" || name == "write") && argsV.size() == 1) {
        if (argsV[0]->getType()->isDoubleTy())
            fnName += "Real";
        else if (argsV[0]->getType()->isPointerTy())
            fnName += "String";
    }

    llvm::Function* calleeF = ctx.module.getFunction(fnName);
    if (!calleeF)
        return nullptr;

    if (calleeF->arg_size() != args.size())
        return nullptr;

    if (calleeF->getReturnType()->isVoidTy())
        return ctx.builder.CreateCall(calleeF, argsV);

    return ctx.builder.CreateCall(calleeF, argsV, "calltmp");
}

} // namespace milan
