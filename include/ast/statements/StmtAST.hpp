#pragma once

#include "CodegenContext.hpp"

namespace milan {

class StmtAST {
public:
    virtual ~StmtAST() = default;
    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;
};

} // namespace milan
