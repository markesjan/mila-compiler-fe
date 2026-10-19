#pragma once

#include <string>
#include <vector>

#include "DeclAST.hpp"
#include "Type.hpp"

namespace milan {

class ParameterDeclAST : public DeclAST {
public:
    ParameterDeclAST(SourceLocation loc, std::vector<std::string> names, Type type);
    llvm::Value* codegen(CodegenContext& ctx) override;

    const std::vector<std::string>& getNames() const noexcept;
    const Type& getType() const noexcept;

private:
    std::vector<std::string> names;
    Type type;
};

} // namespace milan
