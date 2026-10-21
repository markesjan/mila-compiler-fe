#pragma once

#include <string>

#include "ExprAST.hpp"

namespace milan {

class ArrayAccessExprAST : public ExprAST {
public:
    ArrayAccessExprAST(SourceLocation loc, std::string name, UPtrExpr idx);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    UPtrExpr idx;
};

} // namespace milan
