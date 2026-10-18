#pragma once

#include <string>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

namespace milan {

class TypeImpl {
public:
    virtual ~TypeImpl() = default;

    virtual llvm::Type* toLlvmType(llvm::LLVMContext& ctx) const = 0;
    virtual std::string toString() const = 0;
    virtual bool equals(const TypeImpl& other) const = 0;
};

} // namespace milan
