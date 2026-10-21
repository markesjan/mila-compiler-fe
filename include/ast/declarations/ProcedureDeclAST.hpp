/**
 * @file ProcedureDeclAST.hpp
 * @brief AST node representing procedure declarations with local declarations and a statement body.
 */

#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class ProcedureDeclAST : public DeclAST {
public:
    ProcedureDeclAST(SourceLocation loc,
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
