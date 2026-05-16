#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "Type.hpp"

namespace milan {

class ParameterDeclAST : public DeclAST {
public:
    ParameterDeclAST(int row, int col, std::vector<std::string> names, Type type);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<std::string> names;
    Type type;
};

} // namespace milan
