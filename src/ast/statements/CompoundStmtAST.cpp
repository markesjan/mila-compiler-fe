#include "CompoundStmtAST.hpp"

#include <utility>

#include <llvm/IR/Constants.h>

#include "CodegenContext.hpp"

namespace milan {

CompoundStmtAST::CompoundStmtAST(SourceLocation loc, std::vector<UPtrStmt> stmts)
    : StmtAST(loc), stmts(std::move(stmts)) {}

llvm::Value* CompoundStmtAST::codegen(CodegenContext& ctx) {
    for (auto& stmt : stmts) {
        if (!stmt)
            continue;
        llvm::Value* val = stmt->codegen(ctx);
        if (!val)
            return nullptr;
    }

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
