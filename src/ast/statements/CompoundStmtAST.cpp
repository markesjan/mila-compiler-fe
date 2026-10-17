#include "CompoundStmtAST.hpp"

namespace milan {

CompoundStmtAST::CompoundStmtAST(SourceLocation loc, std::vector<UPtrStmt> stmts)
    : StmtAST(loc), stmts(std::move(stmts)) {}

llvm::Value* CompoundStmtAST::codegen(CodegenContext& ctx) {
    for (auto& stmt : stmts) {
        llvm::Value* val = stmt->codegen(ctx);
        if (!val)
            return nullptr;
    }

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
