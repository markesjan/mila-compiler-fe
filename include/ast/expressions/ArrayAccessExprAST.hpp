#pragma once

#include <memory>

#include "ExprAST.hpp"

namespace milan {

class ArrayAccessExprAST : public ExprAST {
public:
    ArrayAccessExprAST(std::size_t idx);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::size_t idx;
};

} // namespace milan
