#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "StmtAST.hpp"

namespace milan {

struct CodegenContext;

class Program {
public:
    Program(std::string name, std::vector<UPtrDecl> globalDecls, UPtrStmt mainBlock);
    void codegen(CodegenContext& ctx);

private:
    std::string name;
    std::vector<UPtrDecl> globalDecls;
    UPtrStmt mainBlock;
};

} // namespace milan
