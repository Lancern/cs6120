#include "bril/Function.h"

#include <memory>
#include <string>
#include <utility>

#include "support/Check.h"

namespace bril {

Value *ValueTable::create(std::string name, const Type *ty) noexcept {
  BRIL_CHECK(!named_values_.contains(name));

  auto value = std::make_unique<Value>(ty, name);
  auto ret = value.get();

  named_values_.emplace(std::move(name), std::move(value));

  return ret;
}

Value *ValueTable::createUnnamed(const Type *ty) noexcept {
  auto value = std::make_unique<Value>(ty);
  auto ret = value.get();
  unnamed_values_.push_back(std::move(value));
  return ret;
}

Value *ValueTable::getOrCreate(std::string_view name, const Type *ty) noexcept {
  auto iter = named_values_.find(name);
  if (iter != named_values_.end()) {
    return iter->second.get();
  }

  return create(std::string(name), ty);
}

Function::Function(std::string name, const Type *ret_ty,
                   std::vector<FunctionParamInfo> params) noexcept
    : name_(std::move(name)), ret_ty_{ret_ty}, params_(std::move(params)), cfg_(this) {
  for (const auto &[param_name, param_ty] : params_) {
    values_.create(param_name, param_ty);
  }
}

}  // namespace bril
