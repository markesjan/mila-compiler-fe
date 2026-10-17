#pragma once

#include <map>
#include <string>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "LoopStack.hpp"
#include "SymbolTable.hpp"
#include "Type.hpp"

namespace milan {

struct CodegenContext {
    llvm::LLVMContext& context;
    llvm::Module& module;
    llvm::IRBuilder<>& builder;
    SymbolTable symbols;
    LoopStack loops;
};

} // namespace milan
