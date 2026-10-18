#include "IntegerType.hpp"

#include <llvm/IR/DerivedTypes.h>

namespace milan {

llvm::Type* IntegerType::toLlvmType(llvm::LLVMContext& ctx) const {
    return llvm::Type::getInt32Ty(ctx);
}

std::string IntegerType::toString() const {
    return "integer";
}

bool IntegerType::equals(const TypeImpl& other) const {
    return dynamic_cast<const IntegerType*>(&other) != nullptr;
}

} // namespace milan
