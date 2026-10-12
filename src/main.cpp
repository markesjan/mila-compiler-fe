#include "Parser.hpp"

#include <print>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    std::ifstream inputFile;
    if (argc > 1 && argv[1][0] != '-') {
        inputFile.open(argv[1]);
        if (!inputFile.is_open()) {
            std::println(stderr, "Error: Could not open file '{}'", argv[1]);
            return 1;
        }
        std::cin.rdbuf(inputFile.rdbuf());
    }

    milan::Parser parser;
    if (!parser.parse())
        return 1;

    parser.generate().print(llvm::outs(), nullptr);

    return 0;
}
