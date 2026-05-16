#pragma once

#include "DeclAST.hpp"
#include "Token.hpp"

namespace milan {

class VariableDeclAST : public DeclAST {
public:
    explicit VariableDeclAST(std::vector<std::string> names, Token type);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<std::string> names;
    Token type;
};

} // namespace milan
