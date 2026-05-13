#include "Parser.hpp"

namespace milan {

Parser::Parser() : MilaContext(), MilaBuilder(MilaContext), MilaModule("mila", MilaContext) {
}

bool Parser::parse() {
    getNextToken();
    return true;
}

const llvm::Module& Parser::generate() {
    // create writeln function
    {
        std::vector<llvm::Type*> Ints(1, llvm::Type::getInt32Ty(MilaContext));
        llvm::FunctionType* FT =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), Ints, false);
        llvm::Function* F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "writeln", MilaModule);
        for (auto& Arg : F->args())
            Arg.setName("x");
    }

    // create main function
    {
        llvm::FunctionType* FT =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), false);
        llvm::Function* MainFunction =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main", MilaModule);

        // block
        llvm::BasicBlock* BB = llvm::BasicBlock::Create(MilaContext, "entry", MainFunction);
        MilaBuilder.SetInsertPoint(BB);

        // call writeln with value from lexel
        MilaBuilder.CreateCall(
            MilaModule.getFunction("writeln"),
            {llvm::ConstantInt::get(MilaContext, llvm::APInt(32, lexer.getNumVal()))});

        // return 0
        MilaBuilder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(MilaContext), 0));
    }

    return this->MilaModule;
}

int Parser::getNextToken() {
    return currTok = lexer.nextToken();
}

} // namespace milan
