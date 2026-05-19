#include "CallStmtAST.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

namespace milan {

CallStmtAST::CallStmtAST(int row, int col, std::string name, std::vector<UPtrExpr> args)
    : StmtAST(row, col), name(std::move(name)), args(std::move(args)) {
}

llvm::Value* CallStmtAST::codegen(CodegenContext& ctx) {
    llvm::Function* calleeF = ctx.module.getFunction(name);
    if (!calleeF)
        return nullptr;

    if (calleeF->arg_size() != args.size())
        return nullptr;

    std::vector<llvm::Value*> argsV;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        llvm::Value* argVal = args[i]->codegen(ctx);
        if (!argVal)
            return nullptr;
        argsV.push_back(argVal);
    }

    ctx.builder.CreateCall(calleeF, argsV);

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
