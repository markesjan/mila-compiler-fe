#include "CallStmtAST.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

namespace milan {

CallStmtAST::CallStmtAST(SourceLocation loc, std::string name, std::vector<UPtrExpr> args)
    : StmtAST(loc), name(std::move(name)), args(std::move(args)) {}

llvm::Value* CallStmtAST::codegen(CodegenContext& ctx) {
    std::vector<llvm::Value*> argsV;
    for (const auto& arg : args) {
        llvm::Value* argVal = arg->codegen(ctx);
        if (!argVal)
            return nullptr;
        argsV.push_back(argVal);
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

    ctx.builder.CreateCall(calleeF, argsV);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
