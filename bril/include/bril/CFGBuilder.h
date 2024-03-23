#pragma once

#include <concepts>
#include <utility>

#include "CFG.h"
#include "Inst.h"

namespace bril {

class CFGBuilder {
public:
  /// Construct a new CFGBuilder object that does not have an insertion point.
  CFGBuilder() noexcept = default;

  /// Construct a new CFGBuilder object that can be used for creating new blocks in the specified
  /// CFG.
  explicit CFGBuilder(CFG *cfg) noexcept : ip_{cfg} {}

  /// Construct a new CFGBuilder object and set its insertion point to the end of the specified
  /// block.
  explicit CFGBuilder(Block *block) noexcept;

  /// Construct a new CFGBuilder object and set its insertion point to the location immediately
  /// before the specified instruction.
  explicit CFGBuilder(Inst *inst) noexcept;

  /// Create a new block.
  ///
  /// This function will not reset the insertion point.
  Block *createBlock() const noexcept;

  /// Create a new instruction of type I and insert it before the insertion point.
  template <std::derived_from<Inst> I, typename ...Args>
  I *createInst(Args &&...args) const noexcept {
    auto *inst = new I(std::forward<Args>(args)...);
    insertInst(inst);
    return inst;
  }

  /// Remove the instruction at the insertion point.
  ///
  /// This function will move the insertion point to the next instruction after the removed
  /// instruction.
  void removeInst() noexcept;

private:
  struct InsertionPoint {
    CFG *cfg{nullptr};
    Block *block{nullptr};
    Inst *inst{nullptr};
  };

  InsertionPoint ip_;

  void checkHasInsertionPoint() const noexcept;

  /// Insert the specified instruction before the insertion point.
  void insertInst(Inst *inst) const noexcept;
};

} // namespace bril
