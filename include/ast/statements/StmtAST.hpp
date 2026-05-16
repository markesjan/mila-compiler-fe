#pragma once

#include "CodegenContext.hpp"

namespace milan {

class StmtAST {
public:
    StmtAST(int row, int col);
    virtual ~StmtAST() = default;
    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;

private:
    int row, col;
};

using UPtrStmt = std::unique_ptr<StmtAST>;

} // namespace milan
