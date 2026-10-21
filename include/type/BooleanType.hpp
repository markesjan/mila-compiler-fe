#pragma once

#include <string>

#include "TypeImpl.hpp"

namespace llvm {
class LLVMContext;
class Type;
} // namespace llvm

namespace milan {

class BooleanType : public TypeImpl {
public:
    llvm::Type* toLlvmType(llvm::LLVMContext& ctx) const override;
    std::string toString() const override;
    bool equals(const TypeImpl& other) const override;
};

} // namespace milan
