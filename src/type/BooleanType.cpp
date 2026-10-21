#include "BooleanType.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

namespace milan {

llvm::Type* BooleanType::toLlvmType(llvm::LLVMContext& ctx) const {
    return llvm::Type::getInt1Ty(ctx);
}

std::string BooleanType::toString() const {
    return "boolean";
}

bool BooleanType::equals(const TypeImpl& other) const {
    return dynamic_cast<const BooleanType*>(&other) != nullptr;
}

} // namespace milan
