#include "ArrayType.hpp"

#include <format>

namespace milan {

ArrayType::ArrayType(std::int32_t low, std::int32_t high, Type elemType)
    : low(low), high(high), elemType(elemType) {}

llvm::Type* ArrayType::toLlvmType(llvm::LLVMContext& ctx) const {
    llvm::Type* elem = elemType.toLlvmType(ctx);
    return llvm::ArrayType::get(elem, high - low + 1);
}

std::string ArrayType::toString() const {
    return std::format("array [{}..{}] of {}", low, high, elemType.toString());
}

bool ArrayType::equals(const TypeImpl& other) const {
    auto* otherArr = dynamic_cast<const ArrayType*>(&other);
    if (!otherArr)
        return false;

    return low == otherArr->low && high == otherArr->high && elemType == otherArr->elemType;
}

std::int32_t ArrayType::getLow() const {
    return low;
}

std::int32_t ArrayType::getHigh() const {
    return high;
}

const Type& ArrayType::getElemType() const {
    return elemType;
}

} // namespace milan
