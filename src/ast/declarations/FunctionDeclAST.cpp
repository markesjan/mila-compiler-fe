#include "FunctionDeclAST.hpp"

namespace milan {

FunctionDeclAST::FunctionDeclAST(SourceLocation loc,
                                 std::string name,
                                 Type retType,
                                 std::vector<UPtrDecl> params,
                                 std::vector<UPtrDecl> localDecls,
                                 UPtrStmt body)
    : DeclAST(loc),
      name(std::move(name)),
      retType(std::move(retType)),
      params(std::move(params)),
      localDecls(std::move(localDecls)),
      body(std::move(body)) {}

llvm::Value* FunctionDeclAST::codegen(CodegenContext& ctx) {
    return nullptr;
}

} // namespace milan
