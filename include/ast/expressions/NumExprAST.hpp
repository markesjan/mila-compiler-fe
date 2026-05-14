#pragma once

#include "ExprAST.hpp"

namespace milan {

class NumExprAST : public ExprAST {
public:
    explicit NumExprAST(std::int32_t val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
     std::int32_t val;
};

} // namespace milan
