#pragma once

#include <string>

#include "DeclAST.hpp"

namespace milan {

class ConstantDeclAST : public DeclAST {
public:
    ConstantDeclAST(int row, int col, std::string name, int val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    int val;
};

} // namespace milan
