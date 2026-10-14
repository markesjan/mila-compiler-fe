#pragma once

namespace milan {

enum class BaseType { Int, Bool, Real, Void };

class Type {
public:
    Type() = default;
    explicit Type(BaseType base);
    static Type createArray(int low, int high, BaseType elementBase);

    BaseType getBase() const;
    bool isArrayType() const;
    int getLow() const;
    int getHigh() const;

private:
    BaseType base;
    bool isArray;
    int low;
    int high;
};

} // namespace milan
