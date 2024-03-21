#include "bril/Value.h"

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <span>
#include <string>
#include <utility>

#include "bril/Type.h"
#include "support/Check.h"

namespace bril {

Value::Value(const Type *type, std::string name) noexcept : name_(std::move(name)), type_{type} {}

Producer::Producer(Value *value) noexcept : value_{value} { value->producers_.push_back(*this); }

Producer::~Producer() noexcept { value_->producers_.remove(*this); }

Use::Use(Value *value, User *user, std::size_t user_operand_idx_) noexcept
    : value_{value}, user_{user}, user_operand_idx_{user_operand_idx_} {
  value->uses_.push_back(*this);
}

Use::~Use() noexcept { value_->uses_.remove(*this); }

User::User(std::initializer_list<Value *> operand_values) noexcept
    : User(std::span<Value *const>(operand_values.begin(), operand_values.size())) {}

User::User(std::span<Value *const> operand_values) noexcept {
  operands_.reserve(operand_values.size());

  for (std::size_t operand_idx = 0; operand_idx < operand_values.size(); ++operand_idx) {
    auto *v = operand_values[operand_idx];
    operands_.push_back(std::make_unique<Use>(v, this, operand_idx));
  }
}

const Use &User::getOperand(std::size_t index) const noexcept {
  BRIL_CHECK(index <= operands_.size());
  return *operands_[index].get();
}

}  // namespace bril
