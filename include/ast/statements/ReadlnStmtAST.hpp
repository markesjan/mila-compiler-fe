#pragma once

#include <string>

#include "StmtAST.hpp"

namespace milan {

class ReadlnStmtAST : public StmtAST {
public:
    ReadlnStmtAST(int row, int col, std::string varName);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::string varName;
};

} // namespace milan
