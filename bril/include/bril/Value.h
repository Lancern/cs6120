#pragma once

#include <cstddef>
#include <initializer_list>
#include <list>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "Type.h"

namespace bril {

class Producer;
class User;

/// Represent a Bril SSA value.
class Value {
public:
  /// Represent a use of the value.
  class Use {
  public:
    /// Get the user.
    User *getUser() const noexcept { return user_; }

    /// Get the index of the user's operand that uses the value.
    std::size_t getUserOperandIndex() const noexcept { return user_operand_idx_; }

  private:
    Use(User *user, std::size_t user_operand_idx_) noexcept
        : user_{user}, user_operand_idx_{user_operand_idx_} {}

    User *user_;
    std::size_t user_operand_idx_;
  };

  /// Construct a new value with the given name.
  Value(const Type *type, std::string name = "") noexcept;

  /// Get the name of this value.
  ///
  /// If this value does not have a name, this function returns an empty string.
  std::string_view getName() const noexcept { return name_; }

  /// Get the type of this value.
  const Type *getType() const noexcept { return type_; }

  /// Determine whether this value has any producers.
  bool hasProducers() const noexcept { return !producers_.empty(); }

  /// Determine whether there are any uses of this value.
  bool hasUses() const noexcept { return !uses_.empty(); }

  /// Get the number of producers of this value.
  std::size_t getNumProducers() const noexcept { return producers_.size(); }

  /// Get the number of uses of this value.
  std::size_t getNumUses() const noexcept { return uses_.size(); }

  /// Get a view of all the producers of this value.
  auto getProducers() const noexcept {
    auto iter_begin = producers_.begin();
    auto iter_end = producers_.end();
    return std::ranges::subrange(iter_begin, iter_end);
  }

  /// Get a view of all the uses of this value.
  auto getUses() const noexcept {
    auto iter_begin = uses_.begin();
    auto iter_end = uses_.end();
    return std::ranges::subrange(iter_begin, iter_end);
  }

private:
  std::string name_;
  const Type *type_;
  std::list<Producer *> producers_;
  std::list<Use> uses_;
};

/// Base class for all value producers.
class Producer {
public:
  /// Construct a new Producer with the value it produces.
  explicit Producer(Value *value) noexcept;

  /// Get the value produced by this producer.
  Value *getValue() const noexcept { return value_; }

private:
  Value *value_;
  std::list<Producer *>::iterator value_producer_iter_;
};

/// Base class for all value users.
///
/// A user may use (i.e. depend on) multiple values.
class User {
public:
  /// An operand of the user.
  class Operand {
  public:
    /// Get the value this operand refers to.
    Value *getValue() const noexcept { return value_; }

  private:
    Value *value_{nullptr};
    std::list<Value::Use>::iterator value_use_iter_;
  };

  /// Determine whether this user has any operands.
  bool hasOperands() const noexcept { return !operands_.empty(); }

  /// Get the number of operands.
  std::size_t getNumOperands() const noexcept { return operands_.size(); }

  /// Get a span of all the operands of this user.
  std::span<const Operand> getOperands() const noexcept { return operands_; }

  /// Get the operand at the specified index.
  const Operand &getOperand(std::size_t index) const noexcept;

protected:
  explicit User(std::initializer_list<Value *> operand_values) noexcept;
  explicit User(std::span<Value *const> operand_values) noexcept;

private:
  std::vector<Operand> operands_;
};

}  // namespace bril
