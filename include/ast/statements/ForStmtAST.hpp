#pragma once

#include <cstdint>
#include <string>

#include "ExprAST.hpp"
#include "StmtAST.hpp"

namespace milan {

enum class TokenKind : std::uint8_t;

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
