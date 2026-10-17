#pragma once

#include <string>

#include "DeclAST.hpp"
#include "ExprAST.hpp"

namespace milan {

class ConstantDeclAST : public DeclAST {
public:
    ConstantDeclAST(SourceLocation loc, std::string name, UPtrExpr val);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    UPtrExpr val;
};

} // namespace milan
