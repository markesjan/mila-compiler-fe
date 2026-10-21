#include "Parser.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "ConstantDeclAST.hpp"
#include "DeclAST.hpp"
#include "Diagnostic.hpp"
#include "FunctionDeclAST.hpp"
#include "ParameterDeclAST.hpp"
#include "ProcedureDeclAST.hpp"
#include "SourceLocation.hpp"
#include "StmtAST.hpp"
#include "Token.hpp"
#include "TokenKind.hpp"
#include "Type.hpp"
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

        auto parseBound = [&]() -> std::optional<std::int32_t> {
            bool isNeg = false;
            if (currTok.is(TokenKind::Minus)) {
                isNeg = true;
                getNextToken();
            }

            if (currTok.isNot(TokenKind::Number)) {
                Diagnostic::error(currTok.location, "Expected number in array range");
                return std::nullopt;
            }

            std::int32_t val = currTok.numberValue;
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

std::vector<UPtrDecl> Parser::parseParameters() {
    std::vector<UPtrDecl> params;

    if (!expect(TokenKind::LParen, "parameters"))
        return {};

    if (currTok.is(TokenKind::RParen)) {
        getNextToken();
        return {};
    }

    while (true) {
        SourceLocation loc = currTok.location;

        if (currTok.isNot(TokenKind::Identifier)) {
            Diagnostic::error(currTok.location, "Expected parameter identifier");
            return {};
        }

        std::vector<std::string> names;
        names.push_back(currTok.text);
        getNextToken();

        while (match(TokenKind::Comma)) {
            if (currTok.isNot(TokenKind::Identifier)) {
                Diagnostic::error(currTok.location,
                                  "Expected identifier after ',' in parameter list");
                return {};
            }
            names.push_back(currTok.text);
            getNextToken();
        }

        if (!expect(TokenKind::Colon, "parameter type"))
            return {};

        auto typeOpt = parseType();
        if (!typeOpt)
            return {};

        params.push_back(std::make_unique<ParameterDeclAST>(loc, std::move(names), *typeOpt));

        if (currTok.is(TokenKind::RParen))
            break;

        if (!expect(TokenKind::Semicolon, "parameter list"))
            return {};
    }

    if (!expect(TokenKind::RParen, "parameters"))
        return {};

    return params;
}

UPtrDecl Parser::parseFunction() {
    SourceLocation loc = currTok.location;

    if (!expect(TokenKind::Function, "function declaration"))
        return nullptr;

    if (currTok.isNot(TokenKind::Identifier)) {
        Diagnostic::error(currTok.location, "Expected function name");
        return nullptr;
    }

    std::string name = currTok.text;
    getNextToken();

    std::vector<UPtrDecl> params;
    if (currTok.is(TokenKind::LParen))
        params = parseParameters();

    if (!expect(TokenKind::Colon, "function return type"))
        return nullptr;

    auto retType = parseType();
    if (!retType)
        return nullptr;

    if (!expect(TokenKind::Semicolon, "function header"))
        return nullptr;

    if (match(TokenKind::Forward)) {
        if (!expect(TokenKind::Semicolon, "forward declaration"))
            return nullptr;

        return std::make_unique<FunctionDeclAST>(loc, std::move(name), *retType, std::move(params),
                                                 std::vector<UPtrDecl>{}, nullptr);
    }

    std::vector<UPtrDecl> localDecls;
    while (currTok.is(TokenKind::Const) || currTok.is(TokenKind::Var)) {
        if (currTok.is(TokenKind::Const)) {
            auto constDecls = parseConstSection();
            std::ranges::move(constDecls, std::back_inserter(localDecls));
        } else if (currTok.is(TokenKind::Var)) {
            auto varDecls = parseVarSection();
            std::ranges::move(varDecls, std::back_inserter(localDecls));
        }
    }

    UPtrStmt body = parseBlock();
    if (!body)
        return nullptr;

    if (!expect(TokenKind::Semicolon, "function declaration"))
        return nullptr;

    return std::make_unique<FunctionDeclAST>(loc, std::move(name), *retType, std::move(params),
                                             std::move(localDecls), std::move(body));
}

UPtrDecl Parser::parseProcedure() {
    SourceLocation loc = currTok.location;

    if (!expect(TokenKind::Procedure, "procedure declaration"))
        return nullptr;

    if (currTok.isNot(TokenKind::Identifier)) {
        Diagnostic::error(currTok.location, "Expected procedure name");
        return nullptr;
    }

    std::string name = currTok.text;
    getNextToken();

    std::vector<UPtrDecl> params;
    if (currTok.is(TokenKind::LParen))
        params = parseParameters();

    if (!expect(TokenKind::Semicolon, "procedure header"))
        return nullptr;

    if (match(TokenKind::Forward)) {
        if (!expect(TokenKind::Semicolon, "forward declaration"))
            return nullptr;

        return std::make_unique<ProcedureDeclAST>(loc, std::move(name), std::move(params),
                                                  std::vector<UPtrDecl>{}, nullptr);
    }

    std::vector<UPtrDecl> localDecls;
    while (currTok.is(TokenKind::Const) || currTok.is(TokenKind::Var)) {
        if (currTok.is(TokenKind::Const)) {
            auto constDecls = parseConstSection();
            std::ranges::move(constDecls, std::back_inserter(localDecls));
        } else if (currTok.is(TokenKind::Var)) {
            auto varDecls = parseVarSection();
            std::ranges::move(varDecls, std::back_inserter(localDecls));
        }
    }

    UPtrStmt body = parseBlock();
    if (!body)
        return nullptr;

    if (!expect(TokenKind::Semicolon, "procedure declaration"))
        return nullptr;

    return std::make_unique<ProcedureDeclAST>(loc, std::move(name), std::move(params),
                                              std::move(localDecls), std::move(body));
}

} // namespace milan
