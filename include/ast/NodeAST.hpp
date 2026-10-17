#pragma once

#include <llvm/IR/Value.h>

#include "CodegenContext.hpp"
#include "SourceLocation.hpp"

namespace milan {

class NodeAST {
public:
    explicit NodeAST(SourceLocation loc);
    virtual ~NodeAST() = default;
    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;

    SourceLocation getLocation() const noexcept { return loc; }

protected:
    SourceLocation loc;
};

} // namespace milan
