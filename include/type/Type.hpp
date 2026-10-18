#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include <string>

#include "TypeImpl.hpp"

namespace milan {

template <typename T>
concept TypeCategory = std::derived_from<T, TypeImpl>;

class Type {
public:
    Type() = default;
    explicit Type(std::shared_ptr<const TypeImpl> impl);

    static Type getInt();
    static Type getReal();
    static Type getBool();
    static Type getVoid();
    static Type createArray(std::int32_t low, std::int32_t high, Type elemType);

    template <TypeCategory T, typename... Args>
    static Type make(Args&&... args) {
        return Type(std::make_shared<T>(std::forward<Args>(args)...));
    }

    llvm::Type* toLlvmType(llvm::LLVMContext& ctx) const;
    std::string toString() const;

    template <TypeCategory T>
    const T* as() const {
        return dynamic_cast<const T*>(impl.get());
    }

    template <TypeCategory T>
    bool is() const {
        return as<T>() != nullptr;
    }

    bool isArrayType() const;
    std::int32_t getLow() const;
    std::int32_t getHigh() const;

    bool operator==(const Type& other) const;

private:
    std::shared_ptr<const TypeImpl> impl;
};

} // namespace milan
