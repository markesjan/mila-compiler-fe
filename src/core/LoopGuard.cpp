#include "LoopGuard.hpp"

namespace milan {

LoopGuard::LoopGuard(LoopStack& stack,
                     llvm::BasicBlock* breakTarget,
                     llvm::BasicBlock* continueTarget)
    : stack(stack) {
    stack.push(breakTarget, continueTarget);
}

LoopGuard::~LoopGuard() {
    stack.pop();
}

} // namespace milan
