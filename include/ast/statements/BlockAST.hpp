#pragma once

#include "StmtAST.hpp"

#include <vector>
#include <memory>

namespace milan {

class BlockAST : public StmtAST {
public:
    explicit BlockAST(std::vector<std::unique_ptr<StmtAST>> stmts);
    llvm::Value* codegen(CodegenContext& ctx) override;

private:
    std::vector<std::unique_ptr<StmtAST>> stmts;
};

} // namespace milan
