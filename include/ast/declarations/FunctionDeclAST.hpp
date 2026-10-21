#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "StmtAST.hpp"
#include "Type.hpp"

namespace milan {

class FunctionDeclAST : public DeclAST {
public:
    FunctionDeclAST(SourceLocation loc,
                    std::string name,
                    Type retType,
                    std::vector<UPtrDecl> params,
                    std::vector<UPtrDecl> localDecls,
                    UPtrStmt body);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    Type retType;
    std::vector<UPtrDecl> params;
    std::vector<UPtrDecl> localDecls;
    UPtrStmt body;
};

} // namespace milan
