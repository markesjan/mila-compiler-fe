#pragma once

#include <memory>
#include <vector>

#include "StmtAST.hpp"

namespace milan {

class CompoundStmtAST : public StmtAST {
public:
    CompoundStmtAST(SourceLocation loc, std::vector<UPtrStmt> stmts);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<UPtrStmt> stmts;
};

} // namespace milan
