#pragma once

#include <llvm/IR/IRBuilder.h>

namespace milan {

struct CodegenContext {
    llvm::LLVMContext& context;
    llvm::Module& module;
    llvm::IRBuilder<>& builder;
};

} // namespace milan
