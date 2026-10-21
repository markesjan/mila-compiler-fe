/**
 * @file VariableExprAST.hpp
 * @brief AST node representing variable and constant identifier references.
 */

#pragma once

#include <string>

#include "ExprAST.hpp"

namespace milan {

class VariableExprAST : public ExprAST {
public:
    VariableExprAST(SourceLocation loc, std::string name);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
};

} // namespace milan
