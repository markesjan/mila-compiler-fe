#include "ParameterDeclAST.hpp"

#include <utility>

namespace milan {

ParameterDeclAST::ParameterDeclAST(SourceLocation loc, std::vector<std::string> names, Type type)
    : DeclAST(loc), names(std::move(names)), type(std::move(type)) {}

llvm::Value* ParameterDeclAST::codegen(CodegenContext& /*ctx*/) {
    return nullptr;
}

const std::vector<std::string>& ParameterDeclAST::getNames() const noexcept {
    return names;
}

const Type& ParameterDeclAST::getType() const noexcept {
    return type;
}

} // namespace milan
