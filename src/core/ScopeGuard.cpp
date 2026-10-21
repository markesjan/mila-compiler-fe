#include "ScopeGuard.hpp"

#include "SymbolTable.hpp"

namespace milan {

ScopeGuard::ScopeGuard(SymbolTable& table) : table(table) {
    table.enterScope();
}

ScopeGuard::~ScopeGuard() {
    table.exitScope();
}

} // namespace milan
