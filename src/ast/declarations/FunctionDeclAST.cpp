#include "FunctionDeclAST.hpp"

#include <cstddef>
#include <utility>
#include <vector>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

#include "CodegenContext.hpp"
#include "ParameterDeclAST.hpp"
#include "ScopeGuard.hpp"
#include "SymbolTable.hpp"

namespace milan {

FunctionDeclAST::FunctionDeclAST(SourceLocation loc,
                                 std::string name,
                                 Type retType,
                                 std::vector<UPtrDecl> params,
                                 std::vector<UPtrDecl> localDecls,
                                 UPtrStmt body)
    : DeclAST(loc),
      name(std::move(name)),
      retType(std::move(retType)),
      params(std::move(params)),
      localDecls(std::move(localDecls)),
      body(std::move(body)) {}

llvm::Value* FunctionDeclAST::codegen(CodegenContext& ctx) {
    std::vector<llvm::Type*> paramLlvmTypes;
    std::vector<std::pair<std::string, Type>> paramInfo;

    for (const auto& p : params) {
        auto* paramDecl = dynamic_cast<ParameterDeclAST*>(p.get());
        if (!paramDecl)
            continue;

        llvm::Type* llvmType = paramDecl->getType().toLlvmType(ctx.context);
        for (const auto& paramName : paramDecl->getNames()) {
            paramLlvmTypes.push_back(llvmType);
            paramInfo.emplace_back(paramName, paramDecl->getType());
        }
    }

    llvm::Function* func = ctx.module.getFunction(name);
    if (!func) {
        llvm::FunctionType* fnType =
            llvm::FunctionType::get(retType.toLlvmType(ctx.context), paramLlvmTypes, false);
        func = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, name, ctx.module);

        std::size_t idx = 0;
        for (auto& arg : func->args()) {
            if (idx < paramInfo.size())
                arg.setName(paramInfo[idx++].first);
        }
    }

    if (!body)
        return func;

    auto* savedBlock = ctx.builder.GetInsertBlock();
    auto savedPoint = ctx.builder.GetInsertPoint();

    llvm::BasicBlock* entryBB = llvm::BasicBlock::Create(ctx.context, "entry", func);
    ctx.builder.SetInsertPoint(entryBB);

    ScopeGuard scopeGuard(ctx.symbols);

    std::size_t argIdx = 0;
    for (auto& arg : func->args()) {
        if (argIdx < paramInfo.size()) {
            const auto& [paramName, pType] = paramInfo[argIdx++];
            llvm::AllocaInst* alloca =
                ctx.builder.CreateAlloca(pType.toLlvmType(ctx.context), nullptr, paramName);
            ctx.builder.CreateStore(&arg, alloca);
            ctx.symbols.insert(paramName, pType, alloca);
        }
    }

    llvm::Type* llvmRetType = retType.toLlvmType(ctx.context);
    llvm::AllocaInst* retAlloca = ctx.builder.CreateAlloca(llvmRetType, nullptr, name);
    ctx.symbols.insert(name, retType, retAlloca);

    for (const auto& decl : localDecls) {
        if (decl)
            decl->codegen(ctx);
    }

    if (body)
        body->codegen(ctx);

    if (!ctx.builder.GetInsertBlock()->getTerminator()) {
        llvm::Value* retVal = ctx.builder.CreateLoad(llvmRetType, retAlloca, "retVal");
        ctx.builder.CreateRet(retVal);
    }

    if (savedBlock)
        ctx.builder.SetInsertPoint(savedBlock, savedPoint);

    return func;
}

} // namespace milan
