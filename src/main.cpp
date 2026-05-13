#include "Parser.hpp"

int main(int argc, char* argv[]) {
    milan::Parser parser;

    if (!parser.parse()) {
        return 1;
    }

    parser.generate().print(llvm::outs(), nullptr);

    return 0;
}
