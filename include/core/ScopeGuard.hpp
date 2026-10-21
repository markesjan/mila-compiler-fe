/**
 * @file ScopeGuard.hpp
 * @brief RAII guard for managing nested lexical scopes in the symbol table.
 */

#pragma once

namespace milan {

class SymbolTable;

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
