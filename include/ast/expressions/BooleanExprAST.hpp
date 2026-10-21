/**
 * @file BooleanExprAST.hpp
 * @brief AST node representing boolean literal values (true/false).
 */

#pragma once

#include "ExprAST.hpp"

namespace milan {

class BooleanExprAST : public ExprAST {
public:
    BooleanExprAST(SourceLocation loc, bool val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    bool val;
};

} // namespace milan
