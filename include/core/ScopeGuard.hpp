#pragma once

#include "SymbolTable.hpp"

namespace milan {

class ScopeGuard {
public:
    explicit ScopeGuard(SymbolTable& table);
    ~ScopeGuard();

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&&) = delete;
    ScopeGuard& operator=(ScopeGuard&&) = delete;

private:
    SymbolTable& table;
};

} // namespace milan
