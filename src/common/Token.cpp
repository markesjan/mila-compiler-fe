#include "Token.hpp"

namespace milan {

std::string_view Token::name() const noexcept {
    return tokenKindToString(kind);
}

} // namespace milan
