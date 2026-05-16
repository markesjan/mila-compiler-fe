#pragma once

#include "NodeAST.hpp"

namespace milan {

class ExprAST : public NodeAST {
public:
    using NodeAST::NodeAST;
};

using UPtrExpr = std::unique_ptr<ExprAST>;

} // namespace milan
