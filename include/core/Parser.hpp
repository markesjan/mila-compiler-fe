#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "Diagnostic.hpp"
#include "ExprAST.hpp"
#include "Lexer.hpp"
#include "Program.hpp"
#include "StmtAST.hpp"

namespace milan {

class Parser {
public:
    Parser();
    ~Parser() = default;

    bool parse();
    const llvm::Module& generate();

private:
    Lexer lexer;
    int currTok;
    std::unique_ptr<Program> programAST;

    llvm::LLVMContext MilaContext;
    llvm::IRBuilder<> MilaBuilder;
    llvm::Module MilaModule;

    int getNextToken();
    bool match(int expectedToken);
    bool parseProgram();
    bool expect(int expectedTok, std::string_view ctx = "");
    std::string formatCurrentToken() const;
    static std::string tokenToString(int tok);

    std::vector<UPtrDecl> parseConstSection();
    std::vector<UPtrDecl> parseVarSection();

    UPtrStmt parseBlock();
    UPtrStmt parseStatement();
    std::vector<UPtrStmt> parseStatements();

    UPtrExpr parseExpression();
    UPtrExpr parseSimpleExpr();
    UPtrExpr parseTerm();
    UPtrExpr parseFactor();
    UPtrExpr parseFactorIdentifier(int row, int col);

    inline static const std::unordered_map<int, std::string_view> tokenNames = {
        {tok_eof, "end of file"}, {tok_identifier, "identifier"},
        {tok_number, "number"},   {tok_begin, "begin"},
        {tok_end, "end"},         {tok_if, "if"},
        {tok_then, "then"},       {tok_else, "else"},
        {tok_while, "while"},     {tok_do, "do"},
        {tok_for, "for"},         {tok_to, "to"},
        {tok_downto, "downto"},   {tok_var, "var"},
        {tok_const, "const"},     {tok_integer, "integer"},
        {tok_assign, ":="},       {tok_notequal, "<>"},
        {tok_lessequal, "<="},    {tok_greaterequal, ">="}};
};

} // namespace milan
