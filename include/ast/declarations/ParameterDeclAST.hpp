#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "Token.hpp"

namespace milan {

class ParameterDeclAST : public DeclAST {
public:
    explicit ParameterDeclAST(std::vector<std::string> names, Token type);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<std::string> names;
    Token type;
};

} // namespace milan
