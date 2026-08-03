#include "Type.hpp"

namespace milan {

Type::Type(BaseType base) : base(base), isArray(false), low(0), high(0) {}

Type Type::createArray(int low, int high, BaseType elementBase) {
    Type t(elementBase);
    t.isArray = true;
    t.low = low;
    t.high = high;
    return t;
}

BaseType Type::getBase() const {
    return base;
}

bool Type::isArrayType() const {
    return isArray;
}

int Type::getLow() const {
    return low;
}

int Type::getHigh() const {
    return high;
}

} // namespace milan
