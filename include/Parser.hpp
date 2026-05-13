#pragma once

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include "Lexer.hpp"

namespace milan {

class Parser {
public:
    Parser();
    ~Parser() = default;

    bool parse();
    const llvm::Module& generate();

private:
    int getNextToken();

    Lexer lexer;
    int currTok;

    llvm::LLVMContext MilaContext;
    llvm::IRBuilder<> MilaBuilder;
    llvm::Module MilaModule;
};

} // namespace milan
