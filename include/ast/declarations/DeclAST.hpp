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
