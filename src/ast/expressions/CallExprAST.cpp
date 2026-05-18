#include "CallExprAST.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

namespace milan {

CallExprAST::CallExprAST(int row, int col, std::string name, std::vector<UPtrExpr> args)
    : ExprAST(row, col), name(std::move(name)), args(std::move(args)) {
}

llvm::Value* CallExprAST::codegen(CodegenContext& ctx) {
    llvm::Function* calleeF = ctx.module.getFunction(name);
    if (!calleeF)
        return nullptr;

    if (calleeF->arg_size() != args.size())
        return nullptr;

    std::vector<llvm::Value*> argsV;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        argsV.push_back(args[i]->codegen(ctx));
        if (!argsV.back())
            return nullptr;
    }

    if (calleeF->getReturnType()->isVoidTy())
        return ctx.builder.CreateCall(calleeF, argsV);

    return ctx.builder.CreateCall(calleeF, argsV, "calltmp");
}

} // namespace milan
