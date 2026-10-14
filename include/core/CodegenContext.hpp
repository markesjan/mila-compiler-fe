#pragma once

#include <map>
#include <string>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "Type.hpp"

namespace milan {

struct CodegenContext {
    llvm::LLVMContext& context;
    llvm::Module& module;
    llvm::IRBuilder<>& builder;
    std::map<std::string, Type> typeTable;
    std::map<std::string, llvm::Value*> symbolTable;
    std::vector<llvm::BasicBlock*> loopBreakBlocks;
    std::vector<llvm::BasicBlock*> loopContinueBlocks;
};

} // namespace milan
