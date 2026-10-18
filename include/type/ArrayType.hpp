#pragma once

#include <cstdint>

#include "Type.hpp"
#include "TypeImpl.hpp"

namespace milan {

class ArrayType : public TypeImpl {
public:
    ArrayType(std::int32_t low, std::int32_t high, Type elemType);

    llvm::Type* toLlvmType(llvm::LLVMContext& ctx) const override;
    std::string toString() const override;
    bool equals(const TypeImpl& other) const override;

    std::int32_t getLow() const;
    std::int32_t getHigh() const;
    const Type& getElemType() const;

private:
    std::int32_t low;
    std::int32_t high;
    Type elemType;
};

} // namespace milan
