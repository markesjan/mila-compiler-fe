#pragma once

#include "CompoundStmtAST.hpp"
#include "CodegenContext.hpp"

namespace milan {

class ProgramAST {
public:
    explicit ProgramAST(const std::string& name, std::unique_ptr<CompoundStmtAST> mainBlock);
    void codegen(CodegenContext& ctx);

private:
    std::string name;
    std::unique_ptr<CompoundStmtAST> mainBlock;
};

} // namespace milan
