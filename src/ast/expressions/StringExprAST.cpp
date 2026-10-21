#include "StringExprAST.hpp"

#include <utility>

#include <llvm/IR/IRBuilder.h>

#include "CodegenContext.hpp"

namespace milan {

StringExprAST::StringExprAST(SourceLocation loc, std::string val)
    : ExprAST(loc), val(std::move(val)) {}

llvm::Value* StringExprAST::codegen(CodegenContext& ctx) {
    return ctx.builder.CreateGlobalString(val, "str");
}

const std::string& StringExprAST::getValue() const noexcept {
    return val;
}

} // namespace milan
