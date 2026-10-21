/**
 * @file ReadlnStmtAST.hpp
 * @brief AST node representing standard input read operations.
 */

#pragma once

#include <string>

#include "StmtAST.hpp"

namespace milan {

class ReadlnStmtAST : public StmtAST {
public:
    ReadlnStmtAST(SourceLocation loc, std::string varName);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string varName;
};

} // namespace milan
