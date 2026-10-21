#pragma once

#include "SourceLocation.hpp"

namespace llvm {
class Value;
} // namespace llvm

namespace milan {

struct CodegenContext;

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
