#pragma once

#include "DeclAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class ProcedureDeclAST : public DeclAST {
public:
    explicit ProcedureDeclAST(std::string name,
                              std::vector<UPtrDecl> params,
                              std::vector<UPtrDecl> localDeclares,
                              UPtrStmt body);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    std::vector<UPtrDecl> params;
    std::vector<UPtrDecl> localDecls;
    UPtrStmt body;
};

} // namespace milan
