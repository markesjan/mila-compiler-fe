#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class CallStmtAST : public StmtAST {
public:
    CallStmtAST(int row, int col, std::string name, std::vector<UPtrExpr> args);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    std::vector<UPtrExpr> args;
};

} // namespace milan
