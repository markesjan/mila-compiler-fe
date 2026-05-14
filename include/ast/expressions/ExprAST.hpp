#pragma once

#include "CodegenContext.hpp"

namespace milan {

class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;
};

} // namespace milan
