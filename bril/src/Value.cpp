#include "bril/Value.h"

#include <cstddef>
#include <string>
#include <utility>

namespace bril {

Value::Value(std::string name) noexcept : name_(std::move(name)) {}

Producer::Producer(Value *value) noexcept : value_{value} {
  // TODO: implement Producer::Producer.
}

User::User(std::size_t num_operands) noexcept : operands_(num_operands) {}

}  // namespace bril
