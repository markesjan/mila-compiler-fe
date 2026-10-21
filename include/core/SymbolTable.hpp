#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Type.hpp"

namespace llvm {
class Value;
} // namespace llvm

namespace milan {

struct Symbol {
    std::string name;
    Type type;
    llvm::Value* value = nullptr;
    bool isConstant = false;
};

class SymbolTable {
public:
    SymbolTable();

    void enterScope();
    void exitScope();
    bool insert(Symbol symbol);
    bool insert(std::string name, Type type, llvm::Value* value, bool isConstant = false);
    Symbol* lookup(std::string_view name);
    const Symbol* lookup(std::string_view name) const;

private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
};

} // namespace milan
