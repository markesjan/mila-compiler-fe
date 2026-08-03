#pragma once

#include <memory>
#include <vector>

#include "StmtAST.hpp"

namespace milan {

class CompoundStmtAST : public StmtAST {
public:
    CompoundStmtAST(int row, int col, std::vector<UPtrStmt> stmts);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<UPtrStmt> stmts;
};

} // namespace milan
