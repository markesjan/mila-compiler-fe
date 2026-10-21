#include "NodeAST.hpp"

namespace milan {

NodeAST::NodeAST(SourceLocation loc) : loc(loc) {}

SourceLocation NodeAST::getLocation() const noexcept {
    return loc;
}

} // namespace milan
