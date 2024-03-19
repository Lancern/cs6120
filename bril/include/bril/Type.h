#pragma once

#include <cstdint>

namespace bril {

/// Represent a Bril type.
///
/// Type objects are interned. One can safely compare two types by just comparing the addresses of
/// the two Type objects that represent the two types.
class Type {
public:
  static const Type *getIntType() noexcept;
  static const Type *getBoolType() noexcept;
  static const Type *getPtrType(const Type *pointee) noexcept;

  bool isInt() const noexcept { return isKind(TypeKind::Int); }
  bool isBool() const noexcept { return isKind(TypeKind::Bool); }
  bool isPtr() const noexcept { return isKind(TypeKind::Ptr); }

  const Type *getPointeeType() const noexcept;

private:
  enum class TypeKind : std::uintptr_t {
    Ptr = 0,
    Int = 1,
    Bool = 2,
  };

  Type(TypeKind kind) noexcept;

  Type(const Type *tag) noexcept;

  const Type *tag_;

  bool isKind(TypeKind kind) const noexcept;
};

} // namespace bril
