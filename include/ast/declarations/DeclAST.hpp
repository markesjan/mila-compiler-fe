/**
 * @file DeclAST.hpp
 * @brief Base class for all declaration nodes in the abstract syntax tree.
 */

#pragma once

#include <memory>

#include "NodeAST.hpp"

namespace milan {

class DeclAST : public NodeAST {
public:
    using NodeAST::NodeAST;
};

using UPtrDecl = std::unique_ptr<DeclAST>;

} // namespace milan
