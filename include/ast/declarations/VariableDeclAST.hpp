/**
 * @file VariableDeclAST.hpp
 * @brief AST node representing local or global variable declarations and their memory allocation.
 */

#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "Type.hpp"

namespace milan {

class VariableDeclAST : public DeclAST {
public:
    VariableDeclAST(SourceLocation loc, std::vector<std::string> names, Type type);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<std::string> names;
    Type type;
};

} // namespace milan
