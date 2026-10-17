#include "LoopStack.hpp"

namespace milan {

void LoopStack::push(llvm::BasicBlock* breakTarget, llvm::BasicBlock* continueTarget) {
    stack.emplace_back(breakTarget, continueTarget);
}

void LoopStack::pop() {
    if (!stack.empty())
        stack.pop_back();
}

llvm::BasicBlock* LoopStack::getBreakTarget() const {
    if (stack.empty())
        return nullptr;

    return stack.back().breakTarget;
}

llvm::BasicBlock* LoopStack::getContinueTarget() const {
    if (stack.empty())
        return nullptr;

    return stack.back().continueTarget;
}

bool LoopStack::empty() const {
    return stack.empty();
}

} // namespace milan
