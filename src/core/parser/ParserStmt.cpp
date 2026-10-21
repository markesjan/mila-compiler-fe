#include "Parser.hpp"

#include <format>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AssignStmtAST.hpp"
#include "BinaryOpExprAST.hpp"
#include "BreakStmtAST.hpp"
#include "CallStmtAST.hpp"
#include "CompoundStmtAST.hpp"
#include "ContinueStmtAST.hpp"
#include "Diagnostic.hpp"
#include "EmptyStmtAST.hpp"
#include "ExitStmtAST.hpp"
#include "ExprAST.hpp"
#include "ForStmtAST.hpp"
#include "IfStmtAST.hpp"
#include "NumberExprAST.hpp"
#include "ReadlnStmtAST.hpp"
#include "SourceLocation.hpp"
#include "StmtAST.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"
#include "VariableExprAST.hpp"
#include "WhileStmtAST.hpp"

namespace milan {

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

} // namespace milan
