#pragma once

#include <string>
#include <vector>
#include <memory>

#include "DeclAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class ProcedureDeclAST : public DeclAST {
public:
    ProcedureDeclAST(int row,
                     int col,
                     std::string name,
                     std::vector<UPtrDecl> params,
                     std::vector<UPtrDecl> localDecls,
                     UPtrStmt body);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    std::vector<UPtrDecl> params;
    std::vector<UPtrDecl> localDecls;
    UPtrStmt body;
};

} // namespace milan
