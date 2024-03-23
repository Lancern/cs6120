#pragma once

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/options.hpp>

#include "Inst.h"

namespace il = boost::intrusive;

namespace bril {

class CFG;
class CFGBuilder;
class Function;
class Inst;

class BlockInstTag;
class CFGBlockTag;

/// A basic block within a CFG.
///
/// A basic block owns all instructions that are added to the block.
class Block : il::list_base_hook<il::tag<CFGBlockTag>> {
public:
  Block() noexcept = default;

  Block(const Block &) = delete;
  Block(Block &&) = delete;

  ~Block() noexcept;

  Block &operator=(const Block &) = delete;
  Block &operator=(Block &&) = delete;

  /// Get the CFG this block has been added to.
  CFG *getCFG() const noexcept { return cfg_; }

  /// Get the function this block has been added to.
  Function *getFunction() const noexcept;

  friend class CFGBuilder;
  friend class Inst;

private:
  CFG *cfg_{nullptr};
  il::list<Inst, il::base_hook<il::list_base_hook<il::tag<BlockInstTag>>>> inst_;
};

/// A control flow graph (CFG).
class CFG {
public:
  /// Construct a new CFG object.
  explicit CFG(Function *func) noexcept : func_{func} {}

  CFG(const CFG &) = delete;
  CFG(CFG &&) = delete;

  ~CFG() noexcept;

  CFG &operator=(const CFG &) = delete;
  CFG &operator=(CFG &&) = delete;

  /// Get the entry block of this CFG.
  Block *getEntryBlock() const noexcept;

  /// Get the function that this CFG belongs to.
  Function *getFunction() const noexcept { return func_; }

  friend class CFGBuilder;

private:
  Function *func_;
  il::list<Block, il::base_hook<il::list_base_hook<il::tag<CFGBlockTag>>>> blocks_;
};

} // namespace bril
