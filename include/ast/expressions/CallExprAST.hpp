/**
 * @file CallExprAST.hpp
 * @brief AST node representing function calls returning a value.
 */

#pragma once

#include <string>
#include <vector>

#include "ExprAST.hpp"

namespace milan {

class CallExprAST : public ExprAST {
public:
    CallExprAST(SourceLocation loc, std::string name, std::vector<UPtrExpr> args);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    std::vector<UPtrExpr> args;
};

} // namespace milan
