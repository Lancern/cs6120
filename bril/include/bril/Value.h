#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/options.hpp>

#include "Type.h"

namespace il = boost::intrusive;

namespace bril {

class Producer;
class Use;
class User;

class ValueProducerTag;
class ValueUseTag;

/// Represent a Bril SSA value.
class Value {
public:
  /// Construct a new value with the given name.
  Value(const Type *type, std::string name = "") noexcept;

  Value(const Value &) = delete;
  Value(Value &&) = delete;

  Value &operator=(const Value &) = delete;
  Value &operator=(Value &&) = delete;

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

  friend class Producer;
  friend class Use;

private:
  std::string name_;
  const Type *type_;
  il::list<Producer, il::base_hook<il::list_base_hook<il::tag<ValueProducerTag>>>> producers_;
  il::list<Use, il::base_hook<il::list_base_hook<il::tag<ValueUseTag>>>> uses_;
};

/// Base class for all value producers.
class Producer : public il::list_base_hook<il::tag<ValueProducerTag>> {
public:
  Producer(const Producer &) = delete;
  Producer(Producer &&) = delete;

  Producer &operator=(const Producer &) = delete;
  Producer &operator=(Producer &&) = delete;

  /// Get the value produced by this producer.
  Value *getValue() const noexcept { return value_; }

protected:
  /// Construct a new Producer with the value it produces.
  explicit Producer(Value *value) noexcept;

  ~Producer() noexcept;

private:
  Value *value_;
};

/// Represent a use of a value.
class Use : public il::list_base_hook<il::tag<ValueUseTag>> {
public:
  /// Construct a new Use object.
  Use(Value *value, User *user, std::size_t user_operand_idx_) noexcept;

  Use(const Use &) = delete;
  Use(Use &&) = delete;

  ~Use() noexcept;

  Use &operator=(const Use &) = delete;
  Use &operator=(Use &&) = delete;

  /// Get the value used.
  Value *getValue() const noexcept { return value_; }

  /// Get the user.
  User *getUser() const noexcept { return user_; }

  /// Get the index of the user's operand that uses the value.
  std::size_t getUserOperandIndex() const noexcept { return user_operand_idx_; }

private:
  Value *value_;
  User *user_;
  std::size_t user_operand_idx_;
};

/// Base class for all value users.
///
/// A user may use (i.e. depend on) multiple values.
class User {
public:
  /// Determine whether this user has any operands.
  bool hasOperands() const noexcept { return !operands_.empty(); }

  /// Get the number of operands.
  std::size_t getNumOperands() const noexcept { return operands_.size(); }

  /// Get a view of all the operands of this user.
  auto getOperands() const noexcept {
    return std::views::all(operands_) |
           std::views::transform(
               [](const auto &operand) -> const auto & { return *operand.get(); });
  }

  /// Get the operand at the specified index.
  const Use &getOperand(std::size_t index) const noexcept;

protected:
  User(std::initializer_list<Value *> operand_values) noexcept;
  explicit User(std::span<Value *const> operand_values) noexcept;

private:
  std::vector<std::unique_ptr<Use>> operands_;
};

}  // namespace bril
