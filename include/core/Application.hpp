/**
 * @file Application.hpp
 * @brief Main compiler driver application orchestrating parsing, semantic validation, and code
 * generation.
 */

#pragma once

#include <iostream>
#include <string_view>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace milan {

class Application {
public:
    Application(int argc, char* argv[]);
    Application();
    ~Application() = default;

    int run();
    int run(std::istream& input);

private:
    std::vector<std::string_view> args;
    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    void setupBuiltins();
};

} // namespace milan
