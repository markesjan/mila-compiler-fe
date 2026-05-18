#include "CompoundStmtAST.hpp"

namespace milan {

CompoundStmtAST::CompoundStmtAST(int row, int col, std::vector<UPtrStmt> stmts)
    : StmtAST(row, col), stmts(std::move(stmts)) {
}

llvm::Value* CompoundStmtAST::codegen(CodegenContext& ctx) {
    for (auto& stmt : stmts) {
        llvm::Value* val = stmt->codegen(ctx);
        if (!val)
            return nullptr;
    }

    return llvm::ConstantInt::getTrue(ctx.context);
}

} // namespace milan
