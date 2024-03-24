#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Function.h"
#include "support/StringHash.h"

namespace bril {

/// A Bril program.
class Program {
public:
  Program() noexcept = default;

  Program(const Program &) = delete;
  Program(Program &&) = delete;

  ~Program() noexcept = default;

  Program &operator=(const Program &) = delete;
  Program &operator=(Program &&) = delete;

  /// Get the number of functions in this program.
  std::size_t getNumFunctions() const noexcept { return funcs_.size(); }

  /// Get a view of all functions.
  auto getFunctions() const noexcept {
    return std::views::all(funcs_) |
           std::views::transform(
               [](const std::unique_ptr<Function> &up) noexcept { return up.get(); });
  }

  /// Create a new function in the program.
  Function *createFunction(std::string name, const Type *ret_ty,
                           std::vector<FunctionParamInfo> params) noexcept;

  /// Get the function with the specified name.
  ///
  /// If the function does not exist, this function returns a null pointer.
  Function *getFunction(std::string_view name) const noexcept;

private:
  std::vector<std::unique_ptr<Function>> funcs_;
  std::unordered_map<std::string, Function *, sup::StringHash, std::equal_to<>> func_name_idx_;
};

}  // namespace bril
