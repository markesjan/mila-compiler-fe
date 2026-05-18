#pragma once

#include <llvm/IR/IRBuilder.h>
#include <map>
#include <string>

namespace milan {

struct CodegenContext {
    llvm::LLVMContext& context;
    llvm::Module& module;
    llvm::IRBuilder<>& builder;
    std::map<std::string, llvm::Value*> symbolTable;
};

} // namespace milan
