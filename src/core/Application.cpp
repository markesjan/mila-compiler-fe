#include "Application.hpp"

#include <fstream>
#include <iostream>
#include <print>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include "CodegenContext.hpp"
#include "Parser.hpp"
#include "Program.hpp"

namespace milan {

Application::Application(int argc, char* argv[])
    : args(argv ? std::vector<std::string_view>(argv, argv + argc)
                : std::vector<std::string_view>{}),
      context(),
      module("mila", context),
      builder(context) {}

Application::Application() : Application(0, nullptr) {}

int Application::run() {
    if (args.size() > 1) {
        std::ifstream inputFile{std::string(args[1])};
        if (!inputFile.is_open()) {
            std::println(stderr, "Error: Could not open file '{}'", args[1]);
            return 1;
        }
        return run(inputFile);
    }

    return run(std::cin);
}

int Application::run(std::istream& input) {
    Parser parser(input);
    auto program = parser.parse();
    if (!program)
        return 1;

    setupBuiltins();

    CodegenContext ctx{context, module, builder, {}, {}};
    program->codegen(ctx);

    module.print(llvm::outs(), nullptr);
    return 0;
}

void Application::setupBuiltins() {
    // Create readln function
    {
        std::vector<llvm::Type*> ptrs(1, llvm::PointerType::getUnqual(context));
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context), ptrs, false);
        llvm::Function* fn =
            llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "readln", module);

        for (auto& arg : fn->args())
            arg.setName("x");
    }

    // Create writeln function
    {
        std::vector<llvm::Type*> ints(1, llvm::Type::getInt32Ty(context));
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context), ints, false);
        llvm::Function* fn =
            llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "writeln", module);

        for (auto& arg : fn->args())
            arg.setName("x");
    }

    // Create readlnReal function
    {
        std::vector<llvm::Type*> ptrs(1, llvm::PointerType::getUnqual(context));
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context), ptrs, false);
        llvm::Function* fn =
            llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "readlnReal", module);

        for (auto& arg : fn->args())
            arg.setName("x");
    }

    // Create writelnReal function
    {
        std::vector<llvm::Type*> doubles(1, llvm::Type::getDoubleTy(context));
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context), doubles, false);
        llvm::Function* fn =
            llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "writelnReal", module);

        for (auto& arg : fn->args())
            arg.setName("x");
    }
}

} // namespace milan
