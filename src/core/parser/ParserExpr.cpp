#include "Parser.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ArrayAccessExprAST.hpp"
#include "BinaryOpExprAST.hpp"
#include "BooleanExprAST.hpp"
#include "CallExprAST.hpp"
#include "ExprAST.hpp"
#include "NumberExprAST.hpp"
#include "RealExprAST.hpp"
#include "SourceLocation.hpp"
#include "StringExprAST.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"
#include "UnaryOpExprAST.hpp"
#include "VariableExprAST.hpp"

namespace milan {

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
            currTok.is(TokenKind::Slash) || currTok.is(TokenKind::Mod) ||
            currTok.is(TokenKind::And)) {
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

    if (currTok.is(TokenKind::RealNumber)) {
        auto realExpr = std::make_unique<RealExprAST>(loc, currTok.realValue);
        getNextToken();
        return realExpr;
    }

    if (currTok.is(TokenKind::StringLiteral)) {
        auto strExpr = std::make_unique<StringExprAST>(loc, currTok.text);
        getNextToken();
        return strExpr;
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
