#include "bril/Inst.h"

#include "bril/Literal.h"
#include "bril/Type.h"
#include "bril/Value.h"
#include "support/Check.h"

namespace bril {

ConstantInst::ConstantInst(Value *value, Literal lit) noexcept : ProducerInst{value}, lit_{lit} {
  BRIL_CHECK(value->getType() == lit.getType());
}

BinOpInst::BinOpInst(BinOp kind, Value *result, Value *lhs, Value *rhs) noexcept
    : ProducerInst{result}, User{lhs, rhs}, kind_{kind} {
  switch (kind) {
    case BinOp::Add:
    case BinOp::Mul:
    case BinOp::Sub:
    case BinOp::Div:
      BRIL_CHECK(lhs->getType()->isInt());
      BRIL_CHECK(rhs->getType()->isInt());
      BRIL_CHECK(result->getType()->isInt());
      break;

    case BinOp::Eq:
    case BinOp::Lt:
    case BinOp::Gt:
    case BinOp::Le:
    case BinOp::Ge:
      BRIL_CHECK(lhs->getType()->isInt());
      BRIL_CHECK(rhs->getType()->isInt());
      BRIL_CHECK(result->getType()->isBool());
      break;

    case BinOp::And:
    case BinOp::Or:
      BRIL_CHECK(lhs->getType()->isBool());
      BRIL_CHECK(rhs->getType()->isBool());
      BRIL_CHECK(result->getType()->isBool());
      break;

    default:
      BRIL_UNREACHABLE();
  }
}

NotInst::NotInst(BinOp kind, Value *result, Value *input) noexcept
    : ProducerInst{result}, User{input} {
  BRIL_CHECK(input->getType()->isBool());
  BRIL_CHECK(result->getType()->isBool());
}

BranchInst::BranchInst(Value *condition, Block *true_target, Block *false_target) noexcept
    : User{condition}, true_target_{true_target}, false_target_{false_target} {
  BRIL_CHECK(condition->getType()->isBool());
}

CallVoidInst::CallVoidInst(Function *callee, std::span<Value *const> args) noexcept
    : User{args}, callee_{callee} {
  // TODO: check the types of call arguments.
}

CallInst::CallInst(Function *callee, Value *result, std::span<Value *const> args) noexcept
    : ProducerInst{result}, User{args}, callee_{callee} {
  // TODO: check the types of call arguments and return value.
}

Value *RetInst::getReturnValue() const noexcept {
  if (!hasOperands()) {
    return nullptr;
  }
  return getOperand(0).getValue();
}

}  // namespace bril
