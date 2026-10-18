#include "Parser.hpp"

#include <optional>
#include <string>
#include <vector>

#include "ConstantDeclAST.hpp"
#include "Diagnostic.hpp"
#include "VariableDeclAST.hpp"

namespace milan {

std::optional<Type> Parser::parseType() {
    if (currTok.is(TokenKind::Integer)) {
        getNextToken();
        return Type::getInt();
    }
    if (currTok.is(TokenKind::Boolean)) {
        getNextToken();
        return Type::getBool();
    }
    if (currTok.is(TokenKind::Real)) {
        getNextToken();
        return Type::getReal();
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

        std::optional<Type> elemType;
        if (currTok.is(TokenKind::Integer)) {
            elemType = Type::getInt();
            getNextToken();
        } else if (currTok.is(TokenKind::Boolean)) {
            elemType = Type::getBool();
            getNextToken();
        } else if (currTok.is(TokenKind::Real)) {
            elemType = Type::getReal();
            getNextToken();
        } else {
            Diagnostic::error(currTok.location, "Unknown array element type");
            return std::nullopt;
        }

        return Type::createArray(*low, *high, *elemType);
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

} // namespace milan
