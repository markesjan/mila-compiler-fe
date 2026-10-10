#include "Parser.hpp"

#include "AllNodes.hpp"

namespace milan {

Parser::Parser() : MilaContext(), MilaBuilder(MilaContext), MilaModule("mila", MilaContext) {}

bool Parser::parse() {
    getNextToken();
    return parseProgram();
}

const llvm::Module& Parser::generate() {
    // Create writeln function
    {
        std::vector<llvm::Type*> Ints(1, llvm::Type::getInt32Ty(MilaContext));
        llvm::FunctionType* FT =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), Ints, false);
        llvm::Function* F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "writeln", MilaModule);
        for (auto& Arg : F->args())
            Arg.setName("x");
    }

    if (programAST) {
        CodegenContext ctx{MilaContext, MilaModule, MilaBuilder, {}, {}, {}};
        programAST->codegen(ctx);
    }

    return this->MilaModule;
}

int Parser::getNextToken() {
    return currTok = lexer.nextToken();
}

bool Parser::match(int expectedToken) {
    if (currTok == expectedToken) {
        getNextToken();
        return true;
    }

    return false;
}

bool Parser::parseProgram() {
    if (currTok != tok_program)
        return false;

    getNextToken();

    if (currTok != tok_identifier)
        return false;

    std::string programName = lexer.getIdentifierStr();
    getNextToken();

    if (!match(';'))
        return false;

    std::vector<UPtrDecl> decls;
    while (currTok == tok_const || currTok == tok_var) {
        if (currTok == tok_const) {
            auto constDecls = parseConstSection();
            std::move(constDecls.begin(), constDecls.end(), std::back_inserter(decls));
        }
        if (currTok == tok_var) {
            auto varDecls = parseVarSection();
            std::move(varDecls.begin(), varDecls.end(), std::back_inserter(decls));
        }
    }

    UPtrStmt mainBlock = parseBlock();
    if (!mainBlock)
        return false;

    if (!match('.'))
        return false;

    this->programAST =
        std::make_unique<Program>(programName, std::move(decls), std::move(mainBlock));

    return true;
}

std::vector<UPtrDecl> Parser::parseConstSection() {
    std::vector<UPtrDecl> decls;

    getNextToken();
    while (currTok == tok_identifier) {
        std::string name = lexer.getIdentifierStr();
        getNextToken();

        if (!match('='))
            return decls;

        auto expr = parseExpression();

        if (!match(';'))
            return decls;

        decls.push_back(std::make_unique<ConstantDeclAST>(0, 0, name, std::move(expr)));
    }

    return decls;
}

std::vector<UPtrDecl> Parser::parseVarSection() {
    std::vector<UPtrDecl> decls;

    getNextToken();
    while (currTok == tok_identifier) {
        std::vector<std::string> names;
        names.push_back(lexer.getIdentifierStr());
        getNextToken();

        while (match(',')) {
            names.push_back(lexer.getIdentifierStr());
            getNextToken();
        }

        if (!match(':'))
            return decls;
        if (!match(tok_integer))
            return decls;
        if (!match(';'))
            return decls;

        decls.push_back(std::make_unique<VariableDeclAST>(0, 0, names, Type(BaseType::Int)));
    }

    return decls;
}

UPtrStmt Parser::parseBlock() {
    int row = 0, col = 0;

    if (currTok != tok_begin)
        return nullptr;

    getNextToken();

    std::vector<UPtrStmt> stmts = parseStatements();

    if (currTok != tok_end)
        return nullptr;

    getNextToken();

    return std::make_unique<CompoundStmtAST>(row, col, std::move(stmts));
}

std::vector<UPtrStmt> Parser::parseStatements() {
    std::vector<UPtrStmt> stmts;

    while (currTok != tok_end && currTok != tok_eof) {
        auto stmt = parseStatement();
        if (!stmt)
            return {};

        stmts.push_back(std::move(stmt));

        if (currTok == ';')
            getNextToken();
        else
            break;
    }

    return stmts;
}

UPtrStmt Parser::parseStatement() {
    int row = 0, col = 0;

    if (currTok == tok_identifier) {
        std::string name = lexer.getIdentifierStr();
        getNextToken();

        // Assignment to array element
        if (currTok == '[') {
            getNextToken();

            auto indexExpr = parseExpression();
            if (!indexExpr)
                return nullptr;

            if (!match(']'))
                return nullptr;

            if (!match(tok_assign))
                return nullptr;

            auto valExpr = parseExpression();
            if (!valExpr)
                return nullptr;

            return std::make_unique<AssignStmtAST>(row, col, name, std::move(valExpr),
                                                   std::move(indexExpr));
        }

        // Assignment to simple variable
        if (currTok == tok_assign) {
            getNextToken();

            auto valExpr = parseExpression();
            if (!valExpr)
                return nullptr;

            return std::make_unique<AssignStmtAST>(row, col, name, std::move(valExpr), nullptr);
        }

        // Procedure call
        std::vector<UPtrExpr> args;
        if (currTok == '(') {
            getNextToken();

            if (currTok != ')') {
                while (true) {
                    auto arg = parseExpression();
                    if (!arg)
                        return nullptr;

                    args.push_back(std::move(arg));

                    if (currTok == ')')
                        break;
                    if (!match(','))
                        return nullptr;
                }
            }

            getNextToken();
        }

        return std::make_unique<CallStmtAST>(row, col, name, std::move(args));
    }

    if (currTok == tok_begin)
        return parseBlock();

    // Empty statement
    return std::make_unique<EmptyStmtAST>(row, col);
}

UPtrExpr Parser::parseExpression() {
    int row = 0, col = 0;

    UPtrExpr lhs = parseSimpleExpr();
    if (!lhs)
        return nullptr;

    bool isRel = false;
    Token relTok = tok_error;

    if (currTok == '=' || currTok == '<' || currTok == '>' || currTok == tok_lessequal ||
        currTok == tok_greaterequal || currTok == tok_notequal) {
        isRel = true;
        relTok = static_cast<Token>(currTok);
        getNextToken();
    }

    if (isRel) {
        UPtrExpr rhs = parseSimpleExpr();
        lhs = std::make_unique<BinaryOpExprAST>(row, col, relTok, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

UPtrExpr Parser::parseSimpleExpr() {
    int row = 0, col = 0;

    bool isNeg = false;
    if (currTok == '-') {
        isNeg = true;
        getNextToken();
    }

    UPtrExpr lhs = parseTerm();
    if (!lhs)
        return nullptr;
    if (isNeg)
        lhs = std::make_unique<UnaryOpExprAST>(row, col, static_cast<Token>('-'), std::move(lhs));

    while (true) {
        if (currTok == '+' || currTok == '-' || currTok == tok_or || currTok == tok_xor) {
            auto op = static_cast<Token>(currTok);
            getNextToken();

            UPtrExpr rhs = parseTerm();
            if (!rhs)
                return nullptr;

            lhs = std::make_unique<BinaryOpExprAST>(row, col, op, std::move(lhs), std::move(rhs));
        } else {
            break;
        }
    }

    return lhs;
}

UPtrExpr Parser::parseTerm() {
    int row = 0, col = 0;

    UPtrExpr lhs = parseFactor();
    if (!lhs)
        return nullptr;

    while (true) {
        if (currTok == '*' || currTok == tok_div || currTok == tok_mod || currTok == tok_and) {
            auto op = static_cast<Token>(currTok);
            getNextToken();

            UPtrExpr rhs = parseFactor();
            if (!rhs)
                return nullptr;

            lhs = std::make_unique<BinaryOpExprAST>(row, col, op, std::move(lhs), std::move(rhs));
        } else {
            break;
        }
    }

    return lhs;
}

UPtrExpr Parser::parseFactor() {
    int row = 0, col = 0;

    if (currTok == tok_identifier)
        return parseFactorIdentifier(row, col);

    if (currTok == tok_number) {
        auto numExpr = std::make_unique<NumberExprAST>(row, col, lexer.getNumVal());
        getNextToken();
        return numExpr;
    }

    if (currTok == tok_not) {
        getNextToken();

        auto factor = parseFactor();
        if (!factor)
            return nullptr;

        return std::make_unique<UnaryOpExprAST>(row, col, tok_not, std::move(factor));
    }

    if (currTok == '(') {
        getNextToken();

        auto expr = parseExpression();
        if (!expr)
            return nullptr;

        if (!match(')'))
            return nullptr;

        return expr;
    }

    return nullptr;
}

UPtrExpr Parser::parseFactorIdentifier(int row, int col) {
    std::string name = lexer.getIdentifierStr();
    getNextToken();

    // Array access
    if (currTok == '[') {
        getNextToken();

        auto indexExpr = parseExpression();
        if (!indexExpr)
            return nullptr;

        if (!match(']'))
            return nullptr;

        return std::make_unique<ArrayAccessExprAST>(row, col, name, std::move(indexExpr));
    }

    // Function call
    if (currTok == '(') {
        getNextToken();

        std::vector<UPtrExpr> args;
        if (currTok != ')') {
            while (true) {
                auto arg = parseExpression();
                if (!arg)
                    return nullptr;

                args.push_back(std::move(arg));

                if (currTok == ')')
                    break;
                if (!match(','))
                    return nullptr;
            }
        }

        getNextToken();
        return std::make_unique<CallExprAST>(row, col, name, std::move(args));
    }

    // Variable
    return std::make_unique<VariableExprAST>(row, col, name);
}

} // namespace milan
