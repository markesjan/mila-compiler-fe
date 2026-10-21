#include "VoidType.hpp"

#include <llvm/IR/Type.h>

namespace milan {

llvm::Type* VoidType::toLlvmType(llvm::LLVMContext& ctx) const {
    return llvm::Type::getVoidTy(ctx);
}

std::string VoidType::toString() const {
    return "void";
}

bool VoidType::equals(const TypeImpl& other) const {
    return dynamic_cast<const VoidType*>(&other) != nullptr;
}

} // namespace milan
