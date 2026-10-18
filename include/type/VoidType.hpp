#pragma once

#include "TypeImpl.hpp"

namespace milan {

class VoidType : public TypeImpl {
public:
    llvm::Type* toLlvmType(llvm::LLVMContext& ctx) const override;
    std::string toString() const override;
    bool equals(const TypeImpl& other) const override;
};

} // namespace milan
