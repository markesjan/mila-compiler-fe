/**
 * @file ExprAST.hpp
 * @brief Base class for all expression nodes producing a value in the abstract syntax tree.
 */

#pragma once

#include <memory>

#include "NodeAST.hpp"

namespace milan {

class ExprAST : public NodeAST {
public:
    using NodeAST::NodeAST;
};

using UPtrExpr = std::unique_ptr<ExprAST>;

} // namespace milan
