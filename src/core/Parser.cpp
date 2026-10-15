#include "Parser.hpp"

#include <format>

#include "AllNodes.hpp"

namespace milan {

Parser::Parser() : MilaContext(), MilaBuilder(MilaContext), MilaModule("mila", MilaContext) {}

bool Parser::parse() {
    getNextToken();
    return parseProgram();
}

const llvm::Module& Parser::generate() {
    // Create readln function
    {
        std::vector<llvm::Type*> ptrs(1, llvm::PointerType::getUnqual(MilaContext));
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), ptrs, false);
        llvm::Function* fn =
            llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "readln", MilaModule);

        for (auto& arg : fn->args())
            arg.setName("x");
    }

    // Create writeln function
    {
        std::vector<llvm::Type*> ints(1, llvm::Type::getInt32Ty(MilaContext));
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(MilaContext), ints, false);
        llvm::Function* fn =
            llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "writeln", MilaModule);

        for (auto& arg : fn->args())
            arg.setName("x");
    }

    if (programAST) {
        CodegenContext ctx{MilaContext, MilaModule, MilaBuilder, {}, {}, {}, {}};
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

    if (!expect(';', "program header"))
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

    if (!expect('.', "end of program"))
        return false;

    this->programAST =
        std::make_unique<Program>(programName, std::move(decls), std::move(mainBlock));

    return true;
}

std::optional<Type> Parser::parseType() {
    if (currTok == tok_integer) {
        getNextToken();
        return Type(BaseType::Int);
    }

    if (currTok == tok_boolean) {
        getNextToken();
        return Type(BaseType::Bool);
    }

    if (currTok == tok_array) {
        getNextToken();

        if (!expect('[', "array declaration"))
            return std::nullopt;

        auto parseBound = [&]() -> std::optional<int> {
            bool isNeg = false;
            if (currTok == '-') {
                isNeg = true;
                getNextToken();
            }

            if (currTok != tok_number) {
                Diagnostic::error(lexer.getRow(), lexer.getCol(), "Expected number in array range");
                return std::nullopt;
            }

            int val = lexer.getNumVal();
            if (isNeg)
                val = -val;

            getNextToken();
            return val;
        };

        auto low = parseBound();
        if (!low)
            return std::nullopt;

        if (!expect(tok_range, "array range"))
            return std::nullopt;

        auto high = parseBound();
        if (!high)
            return std::nullopt;

        if (!expect(']', "array declaration"))
            return std::nullopt;
        if (!expect(tok_of, "array declaration"))
            return std::nullopt;

        BaseType elemType = BaseType::Int;
        if (currTok == tok_integer) {
            elemType = BaseType::Int;
            getNextToken();
        } else if (currTok == tok_boolean) {
            elemType = BaseType::Bool;
            getNextToken();
        } else {
            Diagnostic::error(lexer.getRow(), lexer.getCol(), "Unknown array element type");
            return std::nullopt;
        }

        return Type::createArray(*low, *high, elemType);
    }

    Diagnostic::error(lexer.getRow(), lexer.getCol(), "Expected type in variable declaration");
    return std::nullopt;
}

bool Parser::expect(int expectedTok, std::string_view ctx) {
    if (currTok != expectedTok) {
        std::string msg = ctx.empty()
                              ? std::format("Expected '{}', but got {}", tokenToString(expectedTok),
                                            formatCurrentToken())
                              : std::format("Expected '{}', but got {} in {}",
                                            tokenToString(expectedTok), formatCurrentToken(), ctx);

        Diagnostic::error(lexer.getRow(), lexer.getCol(), msg);
        return false;
    }

    getNextToken();
    return true;
}

std::string Parser::formatCurrentToken() const {
    if (currTok == tok_identifier)
        return std::format("identifier '{}'", lexer.getIdentifierStr());
    if (currTok == tok_number)
        return std::format("number '{}'", lexer.getNumVal());

    return std::format("'{}'", tokenToString(currTok));
}

std::string Parser::tokenToString(int tok) {
    if (tok >= 0 && tok < 128)
        return std::string(1, static_cast<char>(tok));

    auto it = tokenNames.find(tok);
    if (it != tokenNames.end())
        return std::string(it->second);

    return "unknown token (" + std::to_string(tok) + ")";
}

std::vector<UPtrDecl> Parser::parseConstSection() {
    std::vector<UPtrDecl> decls;

    getNextToken();
    while (currTok == tok_identifier) {
        int row = lexer.getRow(), col = lexer.getCol();
        std::string name = lexer.getIdentifierStr();
        getNextToken();

        if (!expect('=', "constant declaration"))
            return decls;

        auto expr = parseExpression();
        if (!expr)
            return decls;

        if (!expect(';', "constant declaration"))
            return decls;

        decls.push_back(std::make_unique<ConstantDeclAST>(row, col, name, std::move(expr)));
    }

    return decls;
}

std::vector<UPtrDecl> Parser::parseVarSection() {
    std::vector<UPtrDecl> decls;

    getNextToken();
    while (currTok == tok_identifier) {
        int row = lexer.getRow(), col = lexer.getCol();
        std::vector<std::string> names;
        names.push_back(lexer.getIdentifierStr());
        getNextToken();

        while (match(',')) {
            if (currTok != tok_identifier) {
                Diagnostic::error(lexer.getRow(), lexer.getCol(),
                                  "Expected identifier after ',' in variable declaration");
                return decls;
            }
            names.push_back(lexer.getIdentifierStr());
            getNextToken();
        }

        if (!expect(':', "variable declaration"))
            return decls;

        auto typeOpt = parseType();
        if (!typeOpt)
            return decls;

        if (!expect(';', "variable declaration"))
            return decls;

        decls.push_back(std::make_unique<VariableDeclAST>(row, col, names, *typeOpt));
    }

    return decls;
}

UPtrStmt Parser::parseBlock() {
    int row = lexer.getRow(), col = lexer.getCol();

    if (currTok != tok_begin)
        return nullptr;

    getNextToken();

    std::vector<UPtrStmt> stmts = parseStatements();

    if (!expect(tok_end, "block"))
        return nullptr;

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
    int row = lexer.getRow(), col = lexer.getCol();

    if (currTok == tok_if)
        return parseIfStatement();
    if (currTok == tok_while)
        return parseWhileStatement();
    if (currTok == tok_for)
        return parseForStatement();

    if (currTok == tok_break) {
        getNextToken();
        return std::make_unique<BreakStmtAST>(row, col);
    }
    if (currTok == tok_continue) {
        getNextToken();
        return std::make_unique<ContinueStmtAST>(row, col);
    }
    if (currTok == tok_exit) {
        getNextToken();
        return std::make_unique<ExitStmtAST>(row, col);
    }

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

        if (name == "inc" || name == "dec") {
            if (!match('('))
                return nullptr;

            if (currTok != tok_identifier) {
                Diagnostic::error(lexer.getRow(), lexer.getCol(),
                                  std::format("Expected variable identifier in {}()", name));
                return nullptr;
            }

            std::string targetVar = lexer.getIdentifierStr();
            getNextToken();

            UPtrExpr stepExpr = nullptr;
            if (match(',')) {
                stepExpr = parseExpression();
                if (!stepExpr)
                    return nullptr;
            } else {
                stepExpr = std::make_unique<NumberExprAST>(row, col, 1);
            }

            if (!expect(')', name))
                return nullptr;

            Token op = (name == "inc") ? static_cast<Token>('+') : static_cast<Token>('-');
            auto varExpr = std::make_unique<VariableExprAST>(row, col, targetVar);
            auto binOp = std::make_unique<BinaryOpExprAST>(row, col, op, std::move(varExpr),
                                                           std::move(stepExpr));
            return std::make_unique<AssignStmtAST>(row, col, targetVar, std::move(binOp), nullptr);
        }

        if (name == "readln") {
            if (!match('('))
                return nullptr;
            if (currTok != tok_identifier)
                return nullptr;

            std::string readVarName = lexer.getIdentifierStr();

            getNextToken();

            if (!expect(')', "readln"))
                return nullptr;

            return std::make_unique<ReadlnStmtAST>(row, col, readVarName);
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

UPtrStmt Parser::parseIfStatement() {
    int row = lexer.getRow(), col = lexer.getCol();

    getNextToken();

    UPtrExpr cond = parseExpression();
    if (!cond)
        return nullptr;

    if (!expect(tok_then, "if statement"))
        return nullptr;

    UPtrStmt thenStmt = parseStatement();
    if (!thenStmt)
        return nullptr;

    UPtrStmt elseStmt = nullptr;
    if (currTok == tok_else) {
        getNextToken();

        elseStmt = parseStatement();
        if (!elseStmt)
            return nullptr;
    }

    return std::make_unique<IfStmtAST>(row, col, std::move(cond), std::move(thenStmt),
                                       std::move(elseStmt));
}

UPtrStmt Parser::parseWhileStatement() {
    int row = lexer.getRow(), col = lexer.getCol();

    getNextToken();

    UPtrExpr cond = parseExpression();
    if (!cond)
        return nullptr;

    if (!expect(tok_do, "while loop"))
        return nullptr;

    UPtrStmt body = parseStatement();
    if (!body)
        return nullptr;

    return std::make_unique<WhileStmtAST>(row, col, std::move(cond), std::move(body));
}

UPtrStmt Parser::parseForStatement() {
    int row = lexer.getRow(), col = lexer.getCol();

    getNextToken();

    if (currTok != tok_identifier) {
        Diagnostic::error(row, col, std::format("Expected variable identifier in for loop"));
        return nullptr;
    }

    std::string targetVar = lexer.getIdentifierStr();
    getNextToken();

    if (!expect(tok_assign, "for loop"))
        return nullptr;

    UPtrExpr startExpr = parseExpression();
    if (!startExpr)
        return nullptr;

    if (currTok != tok_to && currTok != tok_downto) {
        Diagnostic::error(lexer.getRow(), lexer.getCol(),
                          std::format("Expected 'to' or 'downto' in for loop"));
        return nullptr;
    }

    auto direction = static_cast<Token>(currTok);
    getNextToken();

    UPtrExpr stopExpr = parseExpression();
    if (!stopExpr)
        return nullptr;

    if (!expect(tok_do, "for loop"))
        return nullptr;

    UPtrStmt body = parseStatement();
    if (!body)
        return nullptr;

    return std::make_unique<ForStmtAST>(row, col, std::move(targetVar), std::move(startExpr),
                                        std::move(stopExpr), direction, std::move(body));
}

UPtrExpr Parser::parseExpression() {
    int row = lexer.getRow(), col = lexer.getCol();

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
    int row = lexer.getRow(), col = lexer.getCol();

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
    int row = lexer.getRow(), col = lexer.getCol();

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
    int row = lexer.getRow(), col = lexer.getCol();

    if (currTok == tok_identifier)
        return parseFactorIdentifier(row, col);

    if (currTok == tok_number) {
        auto numExpr = std::make_unique<NumberExprAST>(row, col, lexer.getNumVal());
        getNextToken();
        return numExpr;
    }

    if (currTok == tok_true || currTok == tok_false) {
        bool val = (currTok == tok_true);
        getNextToken();
        return std::make_unique<BooleanExprAST>(row, col, val);
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

        if (!expect(')', "expression"))
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

        if (!expect(']', "array index"))
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
