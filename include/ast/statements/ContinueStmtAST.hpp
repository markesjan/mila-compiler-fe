/**
 * @file ContinueStmtAST.hpp
 * @brief AST node representing loop iteration skip statements (continue).
 */

#pragma once

#include "StmtAST.hpp"

namespace milan {

class ContinueStmtAST : public StmtAST {
public:
    explicit ContinueStmtAST(SourceLocation loc);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
