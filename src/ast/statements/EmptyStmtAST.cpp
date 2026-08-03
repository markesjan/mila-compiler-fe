#include "EmptyStmtAST.hpp"

#include <llvm/IR/Constants.h>

namespace milan {

EmptyStmtAST::EmptyStmtAST(int row, int col) : StmtAST(row, col) {}

llvm::Value* EmptyStmtAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
