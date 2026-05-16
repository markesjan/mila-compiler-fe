#pragma once

#include <memory>

#include "ExprAST.hpp"
#include "StmtAST.hpp"
#include "Token.hpp"

namespace milan {

class ForStmtAST : public StmtAST {
public:
    explicit ForStmtAST(std::string name,
                        UPtrExpr start,
                        UPtrExpr stop,
                        Token direction,
                        UPtrStmt loopStmt);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    UPtrExpr start;
    UPtrExpr stop;
    Token direction;
    UPtrStmt loopStmt;
};

} // namespace milan
