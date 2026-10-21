/**
 * @file TypeImpl.hpp
 * @brief Polymorphic base interface defining common operations for compiler type implementations.
 */

#pragma once

#include <string>

namespace llvm {
class LLVMContext;
class Type;
} // namespace llvm

namespace milan {

class TypeImpl {
public:
    virtual ~TypeImpl() = default;

    TypeImpl() = default;
    TypeImpl(const TypeImpl&) = delete;
    TypeImpl& operator=(const TypeImpl&) = delete;
    TypeImpl(TypeImpl&&) = delete;
    TypeImpl& operator=(TypeImpl&&) = delete;

    virtual llvm::Type* toLlvmType(llvm::LLVMContext& ctx) const = 0;
    virtual std::string toString() const = 0;
    virtual bool equals(const TypeImpl& other) const = 0;
};

} // namespace milan
