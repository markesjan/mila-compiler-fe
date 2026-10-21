#pragma once

#include <vector>

namespace llvm {
class BasicBlock;
} // namespace llvm

namespace milan {

struct LoopTarget {
    llvm::BasicBlock* breakTarget = nullptr;
    llvm::BasicBlock* continueTarget = nullptr;
};

class LoopStack {
public:
    void push(llvm::BasicBlock* breakTarget, llvm::BasicBlock* continueTarget);
    void pop();
    llvm::BasicBlock* getBreakTarget() const;
    llvm::BasicBlock* getContinueTarget() const;
    bool empty() const;

private:
    std::vector<LoopTarget> stack;
};

} // namespace milan
