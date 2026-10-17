#pragma once

#include <memory>
#include <string>

#include "ExprAST.hpp"
#include "StmtAST.hpp"
#include "TokenKind.hpp"

namespace milan {

class ForStmtAST : public StmtAST {
public:
    ForStmtAST(SourceLocation loc,
               std::string name,
               UPtrExpr start,
               UPtrExpr stop,
               TokenKind direction,
               UPtrStmt loopStmt);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    UPtrExpr start;
    UPtrExpr stop;
    TokenKind direction;
    UPtrStmt loopStmt;
};

} // namespace milan
