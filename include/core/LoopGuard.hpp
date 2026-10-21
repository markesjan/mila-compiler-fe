#pragma once

namespace llvm {
class BasicBlock;
} // namespace llvm

namespace milan {

class LoopStack;

class LoopGuard {
public:
    LoopGuard(LoopStack& stack, llvm::BasicBlock* breakTarget, llvm::BasicBlock* continueTarget);
    ~LoopGuard();

    LoopGuard(const LoopGuard&) = delete;
    LoopGuard& operator=(const LoopGuard&) = delete;
    LoopGuard(LoopGuard&&) = delete;
    LoopGuard& operator=(LoopGuard&&) = delete;

private:
    LoopStack& stack;
};

} // namespace milan
