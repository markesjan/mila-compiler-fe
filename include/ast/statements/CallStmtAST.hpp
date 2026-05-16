#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class CallStmtAST : public StmtAST {
public:
    explicit CallStmtAST(std::string name, std::vector<UPtrExpr> args);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    std::vector<UPtrExpr> args;
};

} // namespace milan
