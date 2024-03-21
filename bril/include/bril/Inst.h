#pragma once

#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/options.hpp>

#include "Literal.h"
#include "Value.h"

namespace il = boost::intrusive;

namespace bril {

class Block;
class CFG;
class Function;

struct BlockInstTag;

/// Base class for all Bril instructions.
class Inst : il::list_base_hook<il::tag<BlockInstTag>> {
public:
  Inst(const Inst &) = delete;
  Inst(Inst &&) = delete;

  virtual ~Inst() noexcept = default;

  Inst &operator=(const Inst &) = delete;
  Inst &operator=(Inst &&) = delete;

  /// Get the block that this instruction has been added to.
  Block *getBlock() const noexcept { return block_; }

  /// Get the CFG that this instruction has been added to.
  CFG *getCFG() const noexcept;

  /// Get the function that this instruction has been added to.
  Function *getFunction() const noexcept;

protected:
  Inst() noexcept = default;

private:
  Block *block_{nullptr};
};

/// Base class for all Bril instructions that produces a value.
class ProducerInst : public Inst, public Producer {
protected:
  explicit ProducerInst(Value *value) noexcept : Producer{value} {}
};

/// The constant instruction.
class ConstantInst : public ProducerInst {
public:
  ConstantInst(Value *value, Literal lit) noexcept;

  const Literal &getLiteral() const noexcept { return lit_; }

private:
  Literal lit_;
};

enum class BinOp {
  Add,
  Mul,
  Sub,
  Div,
  And,
  Or,
  Eq,
  Lt,
  Gt,
  Le,
  Ge,
};

/// A binary operator instruction.
class BinOpInst : public ProducerInst, public User {
public:
  explicit BinOpInst(BinOp kind, Value *result, Value *lhs, Value *rhs) noexcept;

  BinOp getKind() const noexcept { return kind_; }

  Value *getResult() const noexcept { return Producer::getValue(); }
  Value *getLhs() const noexcept { return User::getOperand(0).getValue(); }
  Value *getRhs() const noexcept { return User::getOperand(1).getValue(); }

private:
  BinOp kind_;
};

/// A logical not instruction.
class NotInst : public ProducerInst, public User {
public:
  explicit NotInst(BinOp kind, Value *result, Value *input) noexcept;

  Value *getResult() const noexcept { return Producer::getValue(); }
  Value *getInput() const noexcept { return User::getOperand(0).getValue(); }
};

/// A jump instruction.
class JmpInst : public Inst {
public:
  explicit JmpInst(Block *target) noexcept : target_{target} {}

  Block *getTarget() const noexcept { return target_; }

private:
  Block *target_;
};

/// A conditional branch instruction.
class BranchInst : public Inst, public User {
public:
  BranchInst(Value *condition, Block *true_target, Block *false_target) noexcept;

  Value *getCondition() const noexcept { return User::getOperand(0).getValue(); }
  Block *getTrueTarget() const noexcept { return true_target_; }
  Block *getFalseTarget() const noexcept { return false_target_; }

private:
  Block *true_target_;
  Block *false_target_;
};

/// A call instruction that calls a function without a return value.
class CallVoidInst : public Inst, public User {
public:
  CallVoidInst(Function *callee, std::span<Value *const> args) noexcept;

  Function *getCallee() const noexcept { return callee_; }

private:
  Function *callee_;
};

/// A call instruction that calls a function with a return value.
class CallInst : public ProducerInst, public User {
public:
  CallInst(Function *callee, Value *result, std::span<Value *const> args) noexcept;

  Function *getCallee() const noexcept { return callee_; }

private:
  Function *callee_;
};

/// A return instruction.
class RetInst : public Inst, public User {
public:
  RetInst() noexcept : User{} {}

  explicit RetInst(Value *value) noexcept : User{value} {}

  /// Get the return value.
  ///
  /// If this ret instruction does not have a return value, this function returns a null pointer.
  Value *getReturnValue() const noexcept;
};

}  // namespace bril
