#include "RealType.hpp"

namespace milan {

llvm::Type* RealType::toLlvmType(llvm::LLVMContext& ctx) const {
    return llvm::Type::getDoubleTy(ctx);
}

std::string RealType::toString() const {
    return "real";
}

bool RealType::equals(const TypeImpl& other) const {
    return dynamic_cast<const RealType*>(&other) != nullptr;
}

} // namespace milan
