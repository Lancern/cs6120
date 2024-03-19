#pragma once

#include <cstdint>
#include <variant>

#include "Type.h"

namespace bril {

/// Represent a literal Bril value.
class Literal {
public:
  /// Construct a literal int value.
  explicit Literal(std::int64_t value) noexcept;

  /// Construct a literal bool value.
  explicit Literal(bool value) noexcept;

  bool isInt() const noexcept { return std::holds_alternative<std::int64_t>(value_); }
  bool isBool() const noexcept { return std::holds_alternative<bool>(value_); }

  /// Get the Bril type of this literal value.
  const Type *getType() const noexcept;

  friend bool operator==(const Literal &lhs, const Literal &rhs) noexcept = default;

private:
  std::variant<std::int64_t, bool> value_;
};

} // namespace bril
