#pragma once

#include <string>
#include <memory>

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class AssignStmtAST : public StmtAST {
public:
    AssignStmtAST(int row, int col, std::string name, UPtrExpr val, UPtrExpr idx);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    UPtrExpr val;
    UPtrExpr idx;
};

} // namespace milan
