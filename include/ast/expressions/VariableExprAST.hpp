#pragma once

#include <string>

#include "ExprAST.hpp"

namespace milan {

class VariableExprAST : public ExprAST {
public:
    VariableExprAST(int row, int col, std::string name);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
};

} // namespace milan
