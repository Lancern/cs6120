#include "bril/Value.h"

#include <cstddef>
#include <initializer_list>
#include <span>
#include <string>
#include <utility>

#include "bril/Type.h"
#include "support/Check.h"

namespace bril {

Value::Value(const Type *type, std::string name) noexcept : name_(std::move(name)), type_{type} {}

Producer::Producer(Value *value) noexcept : value_{value} {
  // TODO: implement Producer::Producer.
}

User::User(std::initializer_list<Value *> operand_values) noexcept
    : User(std::span<Value *const>(operand_values.begin(), operand_values.size())) {}

User::User(std::span<Value *const> operand_values) noexcept
    : operands_(operand_values.size()) {
  // TODO: implement User::User
}

const User::Operand &User::getOperand(std::size_t index) const noexcept {
  BRIL_CHECK(index <= operands_.size());
  return operands_[index];
}

}  // namespace bril
