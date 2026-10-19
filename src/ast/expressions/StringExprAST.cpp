#include "StringExprAST.hpp"

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
