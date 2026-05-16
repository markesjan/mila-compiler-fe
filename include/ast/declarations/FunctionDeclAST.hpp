#pragma once
#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "StmtAST.hpp"
#include "Token.hpp"

namespace milan {

class FunctionDeclAST : public DeclAST {
public:
    explicit FunctionDeclAST(std::string name,
                             Token retType,
                             std::vector<UPtrDecl> params,
                             std::vector<UPtrDecl> localDecls,
                             UPtrStmt body);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    Token retType;
    std::vector<UPtrDecl> params;
    std::vector<UPtrDecl> localDecls;
    UPtrStmt body;
};

} // namespace milan
