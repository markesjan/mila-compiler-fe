#include "ProcedureDeclAST.hpp"

namespace milan {

ProcedureDeclAST::ProcedureDeclAST(SourceLocation loc,
                                   std::string name,
                                   std::vector<UPtrDecl> params,
                                   std::vector<UPtrDecl> localDecls,
                                   UPtrStmt body)
    : DeclAST(loc),
      name(std::move(name)),
      params(std::move(params)),
      localDecls(std::move(localDecls)),
      body(std::move(body)) {}

llvm::Value* ProcedureDeclAST::codegen(CodegenContext& ctx) {
    return nullptr;
}

} // namespace milan
