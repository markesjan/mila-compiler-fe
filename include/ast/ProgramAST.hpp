#pragma once

#include "BlockAST.hpp"
#include "CodegenContext.hpp"

namespace milan {

class ProgramAST {
public:
    explicit ProgramAST(const std::string& name, std::unique_ptr<BlockAST> mainBlock);
    void codegen(CodegenContext& ctx);

private:
    std::string name;
    std::unique_ptr<BlockAST> mainBlock;
};

} // namespace milan
