#pragma once

#include <functional>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "CFG.h"
#include "Type.h"
#include "Value.h"
#include "support/StringHash.h"

namespace bril {

/// A value table maintains all the SSA values that are produced and used within a single function.
class ValueTable {
public:
  /// Create a new value with the specified name and type.
  Value *create(std::string name, const Type *ty) noexcept;

  /// Create a new unnamed value with the specified type.
  Value *createUnnamed(const Type *ty) noexcept;

  /// Get or create a value with the specified name and type.
  Value *getOrCreate(std::string_view name, const Type *ty) noexcept;

private:
  std::unordered_map<std::string, std::unique_ptr<Value>, sup::StringHash, std::equal_to<>>
      named_values_;
  std::vector<std::unique_ptr<Value>> unnamed_values_;
};

/// Information of a function parameter.
struct FunctionParamInfo {
  std::string name;
  const Type *ty;
};

/// A Bril function.
class Function {
public:
  /// Construct a new Function object.
  ///
  /// If the function does not have a return type, `ret_ty` should be a null pointer.
  Function(std::string name, const Type *ret_ty, std::vector<FunctionParamInfo> params) noexcept;

  Function(const Function &) = delete;
  Function(Function &&) = delete;

  Function &operator=(const Function &) = delete;
  Function &operator=(Function &&) = delete;

  /// Get the name of this function.
  std::string_view getName() const noexcept { return name_; }

  /// Get the return type of this function.
  ///
  /// If the function does not have a return type, this function returns a null pointer.
  const Type *getReturnType() const noexcept { return ret_ty_; }

  /// Get the parameters of this function.
  std::span<const FunctionParamInfo> getParams() const noexcept { return params_; }

  /// Get the value table that holds the SSA values used in this function.
  ValueTable &getValueTable() noexcept { return values_; }
  const ValueTable &getValueTable() const noexcept { return values_; }

  /// Get the CFG of this function.
  CFG &getCFG() noexcept { return cfg_; }
  const CFG &getCFG() const noexcept { return cfg_; }

private:
  std::string name_;
  const Type *ret_ty_;
  std::vector<FunctionParamInfo> params_;
  ValueTable values_;
  CFG cfg_;
};

}  // namespace bril
