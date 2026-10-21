/**
 * @file EmptyStmtAST.hpp
 * @brief AST node representing an empty statement.
 */

#pragma once

#include "StmtAST.hpp"

namespace milan {

class EmptyStmtAST : public StmtAST {
public:
    explicit EmptyStmtAST(SourceLocation loc);
    llvm::Value* codegen(CodegenContext& ctx) override;
};

} // namespace milan
