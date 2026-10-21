#include "SymbolTable.hpp"

#include <ranges>
#include <utility>

namespace milan {

SymbolTable::SymbolTable() {
    enterScope();
}

void SymbolTable::enterScope() {
    scopes.emplace_back();
}

void SymbolTable::exitScope() {
    if (scopes.size() > 1)
        scopes.pop_back();
}

bool SymbolTable::insert(Symbol symbol) {
    auto [_, inserted] = scopes.back().emplace(symbol.name, std::move(symbol));
    return inserted;
}

bool SymbolTable::insert(std::string name, Type type, llvm::Value* value, bool isConstant) {
    return insert(Symbol{std::move(name), type, value, isConstant});
}

Symbol* SymbolTable::lookup(std::string_view name) {
    for (auto& scope : std::views::reverse(scopes)) {
        auto found = scope.find(std::string(name));
        if (found != scope.end())
            return &found->second;
    }

    return nullptr;
}

const Symbol* SymbolTable::lookup(std::string_view name) const {
    for (auto& scope : std::views::reverse(scopes)) {
        auto found = scope.find(std::string(name));
        if (found != scope.end())
            return &found->second;
    }

    return nullptr;
}

} // namespace milan
