/**
 * @file NodeAST.hpp
 * @brief Abstract base class for all nodes in the abstract syntax tree.
 */

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

    NodeAST(const NodeAST&) = delete;
    NodeAST& operator=(const NodeAST&) = delete;
    NodeAST(NodeAST&&) = default;
    NodeAST& operator=(NodeAST&&) = default;

    virtual llvm::Value* codegen(CodegenContext& ctx) = 0;

    SourceLocation getLocation() const noexcept;

protected:
    SourceLocation loc;
};

} // namespace milan
