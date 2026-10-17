#include "EmptyStmtAST.hpp"

#include <llvm/IR/Constants.h>

namespace milan {

EmptyStmtAST::EmptyStmtAST(SourceLocation loc) : StmtAST(loc) {}

llvm::Value* EmptyStmtAST::codegen(CodegenContext& ctx) {
    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
