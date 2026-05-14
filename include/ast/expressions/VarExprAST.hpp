#pragma once

#include <string>

#include "ExprAST.hpp"

namespace milan {

class VarExprAST : public ExprAST {
public:
    explicit VarExprAST(const std::string& name);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
};

} // namespace milan
