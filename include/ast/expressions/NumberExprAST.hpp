/**
 * @file NumberExprAST.hpp
 * @brief AST node representing 32-bit integer literals.
 */

#pragma once

#include <cstdint>

#include "ExprAST.hpp"

namespace milan {

class NumberExprAST : public ExprAST {
public:
    NumberExprAST(SourceLocation loc, std::int32_t val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::int32_t val;
};

} // namespace milan
