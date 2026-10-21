/**
 * @file ExitStmtAST.hpp
 * @brief AST node representing early exit from functions and procedures.
 */

#pragma once

#include "StmtAST.hpp"

namespace milan {

class ExitStmtAST : public StmtAST {
public:
    explicit ExitStmtAST(SourceLocation loc);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
