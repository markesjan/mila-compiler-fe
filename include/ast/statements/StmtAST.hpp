#pragma once

#include <memory>

#include "NodeAST.hpp"

namespace milan {

class StmtAST : public NodeAST {
public:
    using NodeAST::NodeAST;
};

using UPtrStmt = std::unique_ptr<StmtAST>;

} // namespace milan
