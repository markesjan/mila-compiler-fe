#pragma once

#include <string>
#include <memory>

#include "ExprAST.hpp"

namespace milan {

class ArrayAccessExprAST : public ExprAST {
public:
    ArrayAccessExprAST(int row, int col, std::string name, UPtrExpr idx);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string name;
    UPtrExpr idx;
};

} // namespace milan
