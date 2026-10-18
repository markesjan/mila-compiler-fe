#include "Type.hpp"

#include "ArrayType.hpp"
#include "BooleanType.hpp"
#include "IntegerType.hpp"
#include "RealType.hpp"
#include "VoidType.hpp"

namespace milan {

Type::Type(std::shared_ptr<const TypeImpl> impl) : impl(std::move(impl)) {}

Type Type::getInt() {
    static const auto instance = std::make_shared<IntegerType>();
    return Type(instance);
}

Type Type::getReal() {
    static const auto instance = std::make_shared<RealType>();
    return Type(instance);
}

Type Type::getBool() {
    static const auto instance = std::make_shared<BooleanType>();
    return Type(instance);
}

Type Type::getVoid() {
    static const auto instance = std::make_shared<VoidType>();
    return Type(instance);
}

Type Type::createArray(std::int32_t low, std::int32_t high, Type elemType) {
    return make<ArrayType>(low, high, std::move(elemType));
}

Type Type::fromLlvmType(const llvm::Type* llvmType) {
    if (!llvmType)
        return {};
    if (llvmType->isIntegerTy(32))
        return getInt();
    if (llvmType->isIntegerTy(1))
        return getBool();
    if (llvmType->isDoubleTy())
        return getReal();
    if (llvmType->isVoidTy())
        return getVoid();
    return {};
}

llvm::Type* Type::toLlvmType(llvm::LLVMContext& ctx) const {
    return impl ? impl->toLlvmType(ctx) : nullptr;
}

std::string Type::toString() const {
    return impl ? impl->toString() : "invalid";
}

bool Type::isArrayType() const {
    return is<ArrayType>();
}

std::int32_t Type::getLow() const {
    auto* arr = as<ArrayType>();
    return arr ? arr->getLow() : 0;
}

std::int32_t Type::getHigh() const {
    auto* arr = as<ArrayType>();
    return arr ? arr->getHigh() : 0;
}

bool Type::operator==(const Type& other) const {
    if (impl == other.impl)
        return true;
    if (!impl || !other.impl)
        return false;

    return impl->equals(*other.impl);
}

} // namespace milan
