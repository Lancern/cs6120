#include "bril/Program.h"

#include <memory>
#include <utility>

#include "src/support/Check.h"

namespace bril {

Function *Program::createFunction(std::string name, const Type *ret_ty,
                                  std::vector<FunctionParamInfo> params) noexcept {
  BRIL_CHECK(!func_name_idx_.contains(name));

  auto func = std::make_unique<Function>(name, ret_ty, std::move(params));
  auto func_ptr = func.get();
  funcs_.push_back(std::move(func));
  func_name_idx_.emplace(std::move(name), func_ptr);

  return func_ptr;
}

Function *Program::getFunction(std::string_view name) const noexcept {
  auto iter = func_name_idx_.find(name);
  if (iter == func_name_idx_.end()) {
    return nullptr;
  }
  return iter->second;
}

}  // namespace bril
