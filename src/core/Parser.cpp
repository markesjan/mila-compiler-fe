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

Token Parser::getNextToken() {
    return currTok = lexer.nextToken();
}

bool Parser::match(TokenKind expected) {
    if (currTok.is(expected)) {
        getNextToken();
        return true;
    }

    return false;
}

bool Parser::expect(TokenKind expected, std::string_view ctx) {
    if (currTok.isNot(expected)) {
        std::string msg = ctx.empty()
                              ? std::format("Expected '{}', but got {}",
                                            tokenKindToString(expected), formatCurrentToken())
                              : std::format("Expected '{}', but got {} in {}",
                                            tokenKindToString(expected), formatCurrentToken(), ctx);

        Diagnostic::error(currTok.location, msg);
        return false;
    }

    getNextToken();
    return true;
}

std::string Parser::formatCurrentToken() const {
    if (currTok.is(TokenKind::Identifier))
        return std::format("identifier '{}'", currTok.text);
    if (currTok.is(TokenKind::Number))
        return std::format("number '{}'", currTok.numberValue);

    return std::format("'{}'", currTok.name());
}

bool Parser::parseProgram() {
    if (currTok.isNot(TokenKind::Program))
        return false;

    getNextToken();

    if (currTok.isNot(TokenKind::Identifier))
        return false;

    std::string programName = currTok.text;
    getNextToken();

    if (!expect(TokenKind::Semicolon, "program header"))
        return false;

    std::vector<UPtrDecl> decls;
    while (currTok.is(TokenKind::Const) || currTok.is(TokenKind::Var)) {
        if (currTok.is(TokenKind::Const)) {
            auto constDecls = parseConstSection();
            std::ranges::move(constDecls, std::back_inserter(decls));
        } else if (currTok.is(TokenKind::Var)) {
            auto varDecls = parseVarSection();
            std::ranges::move(varDecls, std::back_inserter(decls));
        }
    }

    UPtrStmt mainBlock = parseBlock();
    if (!mainBlock)
        return false;

    if (!expect(TokenKind::Dot, "end of program"))
        return false;

    this->programAST =
        std::make_unique<Program>(programName, std::move(decls), std::move(mainBlock));

    return true;
}

std::optional<Type> Parser::parseType() {
    if (currTok.is(TokenKind::Integer)) {
        getNextToken();
        return Type(BaseType::Int);
    }

    if (currTok.is(TokenKind::Boolean)) {
        getNextToken();
        return Type(BaseType::Bool);
    }

    if (currTok.is(TokenKind::Array)) {
        getNextToken();

        if (!expect(TokenKind::LBracket, "array declaration"))
            return std::nullopt;

        auto parseBound = [&]() -> std::optional<int> {
            bool isNeg = false;
            if (currTok.is(TokenKind::Minus)) {
                isNeg = true;
                getNextToken();
            }

            if (currTok.isNot(TokenKind::Number)) {
                Diagnostic::error(currTok.location, "Expected number in array range");
                return std::nullopt;
            }

            int val = currTok.numberValue;
            if (isNeg)
                val = -val;

            getNextToken();
            return val;
        };

        auto low = parseBound();
        if (!low)
            return std::nullopt;

        if (!expect(TokenKind::Range, "array range"))
            return std::nullopt;

        auto high = parseBound();
        if (!high)
            return std::nullopt;

        if (!expect(TokenKind::RBracket, "array declaration"))
            return std::nullopt;
        if (!expect(TokenKind::Of, "array declaration"))
            return std::nullopt;

        BaseType elemType = BaseType::Int;
        if (currTok.is(TokenKind::Integer)) {
            elemType = BaseType::Int;
            getNextToken();
        } else if (currTok.is(TokenKind::Boolean)) {
            elemType = BaseType::Bool;
            getNextToken();
        } else {
            Diagnostic::error(currTok.location, "Unknown array element type");
            return std::nullopt;
        }

        return Type::createArray(*low, *high, elemType);
    }

    Diagnostic::error(currTok.location, "Expected type in variable declaration");
    return std::nullopt;
}

std::vector<UPtrDecl> Parser::parseConstSection() {
    std::vector<UPtrDecl> decls;

    getNextToken();
    while (currTok.is(TokenKind::Identifier)) {
        SourceLocation loc = currTok.location;
        std::string name = currTok.text;
        getNextToken();

        if (!expect(TokenKind::Equal, "constant declaration"))
            return decls;

        auto expr = parseExpression();
        if (!expr)
            return decls;

        if (!expect(TokenKind::Semicolon, "constant declaration"))
            return decls;

        decls.push_back(std::make_unique<ConstantDeclAST>(loc, std::move(name), std::move(expr)));
    }

    return decls;
}

std::vector<UPtrDecl> Parser::parseVarSection() {
    std::vector<UPtrDecl> decls;

    getNextToken();
    while (currTok.is(TokenKind::Identifier)) {
        SourceLocation loc = currTok.location;
        std::vector<std::string> names;
        names.push_back(currTok.text);
        getNextToken();

        while (match(TokenKind::Comma)) {
            if (currTok.isNot(TokenKind::Identifier)) {
                Diagnostic::error(currTok.location,
                                  "Expected identifier after ',' in variable declaration");
                return decls;
            }
            names.push_back(currTok.text);
            getNextToken();
        }

        if (!expect(TokenKind::Colon, "variable declaration"))
            return decls;

        auto typeOpt = parseType();
        if (!typeOpt)
            return decls;

        if (!expect(TokenKind::Semicolon, "variable declaration"))
            return decls;

        decls.push_back(std::make_unique<VariableDeclAST>(loc, std::move(names), *typeOpt));
    }

    return decls;
}

UPtrStmt Parser::parseBlock() {
    SourceLocation loc = currTok.location;

    if (currTok.isNot(TokenKind::Begin))
        return nullptr;

    getNextToken();

    std::vector<UPtrStmt> stmts = parseStatements();

    if (!expect(TokenKind::End, "block"))
        return nullptr;

    return std::make_unique<CompoundStmtAST>(loc, std::move(stmts));
}

std::vector<UPtrStmt> Parser::parseStatements() {
    std::vector<UPtrStmt> stmts;

    while (currTok.isNot(TokenKind::End) && currTok.isNot(TokenKind::Eof)) {
        auto stmt = parseStatement();
        if (!stmt)
            return {};

        stmts.push_back(std::move(stmt));

        if (currTok.is(TokenKind::Semicolon))
            getNextToken();
        else
            break;
    }

    return stmts;
}

UPtrStmt Parser::parseStatement() {
    SourceLocation loc = currTok.location;

    if (currTok.is(TokenKind::If))
        return parseIfStatement();
    if (currTok.is(TokenKind::While))
        return parseWhileStatement();
    if (currTok.is(TokenKind::For))
        return parseForStatement();

    if (currTok.is(TokenKind::Break)) {
        getNextToken();
        return std::make_unique<BreakStmtAST>(loc);
    }
    if (currTok.is(TokenKind::Continue)) {
        getNextToken();
        return std::make_unique<ContinueStmtAST>(loc);
    }
    if (currTok.is(TokenKind::Exit)) {
        getNextToken();
        return std::make_unique<ExitStmtAST>(loc);
    }

    if (currTok.is(TokenKind::Identifier)) {
        std::string name = currTok.text;
        getNextToken();

        // Assignment to array element
        if (currTok.is(TokenKind::LBracket)) {
            getNextToken();

            auto indexExpr = parseExpression();
            if (!indexExpr)
                return nullptr;

            if (!match(TokenKind::RBracket))
                return nullptr;

            if (!match(TokenKind::Assign))
                return nullptr;

            auto valExpr = parseExpression();
            if (!valExpr)
                return nullptr;

            return std::make_unique<AssignStmtAST>(loc, std::move(name), std::move(valExpr),
                                                   std::move(indexExpr));
        }

        // Assignment to simple variable
        if (currTok.is(TokenKind::Assign)) {
            getNextToken();

            auto valExpr = parseExpression();
            if (!valExpr)
                return nullptr;

            return std::make_unique<AssignStmtAST>(loc, std::move(name), std::move(valExpr),
                                                   nullptr);
        }

        if (name == "inc" || name == "dec") {
            if (!match(TokenKind::LParen))
                return nullptr;

            if (currTok.isNot(TokenKind::Identifier)) {
                Diagnostic::error(currTok.location,
                                  std::format("Expected variable identifier in {}()", name));
                return nullptr;
            }

            std::string targetVar = currTok.text;
            getNextToken();

            UPtrExpr stepExpr = nullptr;
            if (match(TokenKind::Comma)) {
                stepExpr = parseExpression();
                if (!stepExpr)
                    return nullptr;
            } else {
                stepExpr = std::make_unique<NumberExprAST>(loc, 1);
            }

            if (!expect(TokenKind::RParen, name))
                return nullptr;

            TokenKind op = (name == "inc") ? TokenKind::Plus : TokenKind::Minus;
            auto varExpr = std::make_unique<VariableExprAST>(loc, targetVar);
            auto binOp =
                std::make_unique<BinaryOpExprAST>(loc, op, std::move(varExpr), std::move(stepExpr));
            return std::make_unique<AssignStmtAST>(loc, std::move(targetVar), std::move(binOp),
                                                   nullptr);
        }

        if (name == "readln") {
            if (!match(TokenKind::LParen))
                return nullptr;
            if (currTok.isNot(TokenKind::Identifier))
                return nullptr;

            std::string readVarName = currTok.text;
            getNextToken();

            if (!expect(TokenKind::RParen, "readln"))
                return nullptr;

            return std::make_unique<ReadlnStmtAST>(loc, std::move(readVarName));
        }

        // Procedure call
        std::vector<UPtrExpr> args;
        if (currTok.is(TokenKind::LParen)) {
            getNextToken();

            if (currTok.isNot(TokenKind::RParen)) {
                while (true) {
                    auto arg = parseExpression();
                    if (!arg)
                        return nullptr;

                    args.push_back(std::move(arg));

                    if (currTok.is(TokenKind::RParen))
                        break;
                    if (!match(TokenKind::Comma))
                        return nullptr;
                }
            }

            getNextToken();
        }

        return std::make_unique<CallStmtAST>(loc, std::move(name), std::move(args));
    }

    if (currTok.is(TokenKind::Begin))
        return parseBlock();

    // Empty statement
    return std::make_unique<EmptyStmtAST>(loc);
}

UPtrStmt Parser::parseIfStatement() {
    SourceLocation loc = currTok.location;

    getNextToken();

    UPtrExpr cond = parseExpression();
    if (!cond)
        return nullptr;

    if (!expect(TokenKind::Then, "if statement"))
        return nullptr;

    UPtrStmt thenStmt = parseStatement();
    if (!thenStmt)
        return nullptr;

    UPtrStmt elseStmt = nullptr;
    if (currTok.is(TokenKind::Else)) {
        getNextToken();

        elseStmt = parseStatement();
        if (!elseStmt)
            return nullptr;
    }

    return std::make_unique<IfStmtAST>(loc, std::move(cond), std::move(thenStmt),
                                       std::move(elseStmt));
}

UPtrStmt Parser::parseWhileStatement() {
    SourceLocation loc = currTok.location;

    getNextToken();

    UPtrExpr cond = parseExpression();
    if (!cond)
        return nullptr;

    if (!expect(TokenKind::Do, "while loop"))
        return nullptr;

    UPtrStmt body = parseStatement();
    if (!body)
        return nullptr;

    return std::make_unique<WhileStmtAST>(loc, std::move(cond), std::move(body));
}

UPtrStmt Parser::parseForStatement() {
    SourceLocation loc = currTok.location;

    getNextToken();

    if (currTok.isNot(TokenKind::Identifier)) {
        Diagnostic::error(loc, "Expected variable identifier in for loop");
        return nullptr;
    }

    std::string targetVar = currTok.text;
    getNextToken();

    if (!expect(TokenKind::Assign, "for loop"))
        return nullptr;

    UPtrExpr startExpr = parseExpression();
    if (!startExpr)
        return nullptr;

    if (currTok.isNot(TokenKind::To) && currTok.isNot(TokenKind::Downto)) {
        Diagnostic::error(currTok.location, "Expected 'to' or 'downto' in for loop");
        return nullptr;
    }

    TokenKind direction = currTok.kind;
    getNextToken();

    UPtrExpr stopExpr = parseExpression();
    if (!stopExpr)
        return nullptr;

    if (!expect(TokenKind::Do, "for loop"))
        return nullptr;

    UPtrStmt body = parseStatement();
    if (!body)
        return nullptr;

    return std::make_unique<ForStmtAST>(loc, std::move(targetVar), std::move(startExpr),
                                        std::move(stopExpr), direction, std::move(body));
}

UPtrExpr Parser::parseExpression() {
    SourceLocation loc = currTok.location;

    UPtrExpr lhs = parseSimpleExpr();
    if (!lhs)
        return nullptr;

    bool isRel = false;
    TokenKind relTok = TokenKind::Error;

    if (currTok.is(TokenKind::Equal) || currTok.is(TokenKind::Less) ||
        currTok.is(TokenKind::Greater) || currTok.is(TokenKind::LessEqual) ||
        currTok.is(TokenKind::GreaterEqual) || currTok.is(TokenKind::NotEqual)) {
        isRel = true;
        relTok = currTok.kind;
        getNextToken();
    }

    if (isRel) {
        UPtrExpr rhs = parseSimpleExpr();
        if (!rhs)
            return nullptr;
        lhs = std::make_unique<BinaryOpExprAST>(loc, relTok, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

UPtrExpr Parser::parseSimpleExpr() {
    SourceLocation loc = currTok.location;

    bool isNeg = false;
    if (currTok.is(TokenKind::Minus)) {
        isNeg = true;
        getNextToken();
    }

    UPtrExpr lhs = parseTerm();
    if (!lhs)
        return nullptr;
    if (isNeg)
        lhs = std::make_unique<UnaryOpExprAST>(loc, TokenKind::Minus, std::move(lhs));

    while (true) {
        if (currTok.is(TokenKind::Plus) || currTok.is(TokenKind::Minus) ||
            currTok.is(TokenKind::Or) || currTok.is(TokenKind::Xor)) {
            TokenKind op = currTok.kind;
            getNextToken();

            UPtrExpr rhs = parseTerm();
            if (!rhs)
                return nullptr;

            lhs = std::make_unique<BinaryOpExprAST>(loc, op, std::move(lhs), std::move(rhs));
        } else {
            break;
        }
    }

    return lhs;
}

UPtrExpr Parser::parseTerm() {
    SourceLocation loc = currTok.location;

    UPtrExpr lhs = parseFactor();
    if (!lhs)
        return nullptr;

    while (true) {
        if (currTok.is(TokenKind::Mul) || currTok.is(TokenKind::Div) ||
            currTok.is(TokenKind::Mod) || currTok.is(TokenKind::And)) {
            TokenKind op = currTok.kind;
            getNextToken();

            UPtrExpr rhs = parseFactor();
            if (!rhs)
                return nullptr;

            lhs = std::make_unique<BinaryOpExprAST>(loc, op, std::move(lhs), std::move(rhs));
        } else {
            break;
        }
    }

    return lhs;
}

UPtrExpr Parser::parseFactor() {
    SourceLocation loc = currTok.location;

    if (currTok.is(TokenKind::Identifier))
        return parseFactorIdentifier(loc);

    if (currTok.is(TokenKind::Number)) {
        auto numExpr = std::make_unique<NumberExprAST>(loc, currTok.numberValue);
        getNextToken();
        return numExpr;
    }

    if (currTok.is(TokenKind::True) || currTok.is(TokenKind::False)) {
        bool val = currTok.is(TokenKind::True);
        getNextToken();
        return std::make_unique<BooleanExprAST>(loc, val);
    }

    if (currTok.is(TokenKind::Not)) {
        getNextToken();

        auto factor = parseFactor();
        if (!factor)
            return nullptr;

        return std::make_unique<UnaryOpExprAST>(loc, TokenKind::Not, std::move(factor));
    }

    if (currTok.is(TokenKind::LParen)) {
        getNextToken();

        auto expr = parseExpression();
        if (!expr)
            return nullptr;

        if (!expect(TokenKind::RParen, "expression"))
            return nullptr;

        return expr;
    }

    return nullptr;
}

UPtrExpr Parser::parseFactorIdentifier(SourceLocation loc) {
    std::string name = currTok.text;
    getNextToken();

    // Array access
    if (currTok.is(TokenKind::LBracket)) {
        getNextToken();

        auto indexExpr = parseExpression();
        if (!indexExpr)
            return nullptr;

        if (!expect(TokenKind::RBracket, "array index"))
            return nullptr;

        return std::make_unique<ArrayAccessExprAST>(loc, std::move(name), std::move(indexExpr));
    }

    // Function call
    if (currTok.is(TokenKind::LParen)) {
        getNextToken();

        std::vector<UPtrExpr> args;
        if (currTok.isNot(TokenKind::RParen)) {
            while (true) {
                auto arg = parseExpression();
                if (!arg)
                    return nullptr;

                args.push_back(std::move(arg));

                if (currTok.is(TokenKind::RParen))
                    break;
                if (!match(TokenKind::Comma))
                    return nullptr;
            }
        }

        getNextToken();
        return std::make_unique<CallExprAST>(loc, std::move(name), std::move(args));
    }

    // Variable
    return std::make_unique<VariableExprAST>(loc, std::move(name));
}

} // namespace milan
