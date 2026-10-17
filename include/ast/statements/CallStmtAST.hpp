#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class CallStmtAST : public StmtAST {
public:
    CallStmtAST(SourceLocation loc, std::string name, std::vector<UPtrExpr> args);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    std::vector<UPtrExpr> args;
};

} // namespace milan
