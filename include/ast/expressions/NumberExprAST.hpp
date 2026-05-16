#pragma once

#include "ExprAST.hpp"

namespace milan {

class NumberExprAST : public ExprAST {
public:
    explicit NumberExprAST(std::int32_t val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
     std::int32_t val;
};

} // namespace milan
