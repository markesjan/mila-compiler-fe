#pragma once

#include <string>

#include "ExprAST.hpp"

namespace milan {

class StringExprAST : public ExprAST {
public:
    StringExprAST(SourceLocation loc, std::string val);
    llvm::Value* codegen(CodegenContext& ctx) override;

    const std::string& getValue() const noexcept;

private:
    std::string val;
};

} // namespace milan
