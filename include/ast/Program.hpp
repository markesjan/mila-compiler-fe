#pragma once

#include <string>
#include <memory>
#include <vector>

#include "CodegenContext.hpp"
#include "DeclAST.hpp"
#include "StmtAST.hpp"

namespace milan {

class Program {
public:
    Program(std::string name,
            std::vector<UPtrDecl> globalDecls,
            UPtrStmt mainBlock);
    void codegen(CodegenContext& ctx);

private:
    std::string name;
    std::vector<UPtrDecl> globalDecls;
    UPtrStmt mainBlock;
};

} // namespace milan
