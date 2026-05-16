#pragma once

#include "StmtAST.hpp"

#include <vector>
#include <memory>

namespace milan {

class CompoundStmtAST : public StmtAST {
public:
    explicit CompoundStmtAST(std::vector<UPtrStmt> stmts);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<UPtrStmt> stmts;
};

} // namespace milan
