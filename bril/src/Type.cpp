#include "bril/Type.h"

#include <cstdint>
#include <unordered_map>

#include "support/Check.h"

namespace bril {

namespace {

constexpr std::uintptr_t PTR_MASK = ~static_cast<std::uintptr_t>(alignof(Type));

}  // namespace

const Type *Type::getIntType() noexcept {
  static const Type ty{TypeKind::Int};
  return &ty;
}

const Type *Type::getBoolType() noexcept {
  static const Type ty{TypeKind::Bool};
  return &ty;
}

const Type *Type::getPtrType(const Type *pointee) noexcept {
  static std::unordered_map<const Type *, Type> pointer_types;

  auto iter = pointer_types.find(pointee);
  if (iter == pointer_types.end()) {
    Type ty{pointee};
    iter = pointer_types.emplace(pointee, ty).first;
  }

  return &iter->second;
}

const Type *Type::getPointeeType() const noexcept {
  BRIL_CHECK(isPtr());
  return tag_;
}

Type::Type(TypeKind kind) noexcept
    : tag_{reinterpret_cast<const Type *>(static_cast<std::uintptr_t>(kind))} {
  BRIL_CHECK(kind == TypeKind::Int || kind == TypeKind::Bool);
}

Type::Type(const Type *tag) noexcept : tag_{tag} {}

bool Type::isKind(TypeKind kind) const noexcept {
  return (reinterpret_cast<std::uintptr_t>(tag_) & PTR_MASK) == static_cast<std::uintptr_t>(kind);
}

}  // namespace bril
