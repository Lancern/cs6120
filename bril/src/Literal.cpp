#include "bril/Literal.h"

#include <cstdint>
#include <utility>

namespace bril {

Literal::Literal(std::int64_t value) noexcept : value_{std::in_place_type<std::int64_t>, value} {}

Literal::Literal(bool value) noexcept : value_{std::in_place_type<bool>, value} {}

const Type *Literal::getType() const noexcept {
  if (isInt()) {
    return Type::getIntTy();
  } else {
    return Type::getBoolTy();
  }
}

}  // namespace bril
