#pragma once

#include "CodegenContext.hpp"

namespace milan {

class NodeAST {
public:
    NodeAST(int row, int col);
    virtual ~NodeAST() = default;
    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;

protected:
    int row, col;
};

} // namespace milan
