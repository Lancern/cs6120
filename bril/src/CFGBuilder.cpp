#include "bril/CFGBuilder.h"

#include "bril/CFG.h"
#include "bril/Inst.h"
#include "support/Check.h"

namespace bril {

CFGBuilder::CFGBuilder(Block *block) noexcept : ip_{block->getCFG(), block} { BRIL_CHECK(ip_.cfg); }

CFGBuilder::CFGBuilder(Inst *inst) noexcept : ip_{inst->getCFG(), inst->getBlock(), inst} {
  BRIL_CHECK(ip_.cfg);
  BRIL_CHECK(ip_.block);
}

Block *CFGBuilder::createBlock() const noexcept {
  BRIL_CHECK(ip_.cfg);

  auto *block = new Block{};
  block->cfg_ = ip_.cfg;
  ip_.cfg->blocks_.push_back(*block);
  return block;
}

void CFGBuilder::removeInst() noexcept {
  BRIL_CHECK(ip_.inst);

  auto next_inst_iter = ip_.block->inst_.iterator_to(*ip_.inst);
  Inst *next_inst = nullptr;
  if (next_inst_iter != ip_.block->inst_.end()) {
    next_inst = &*next_inst_iter;
  }

  ip_.block->inst_.remove(*ip_.inst);
  delete ip_.inst;
  ip_.inst = next_inst;
}

void CFGBuilder::checkHasInsertionPoint() const noexcept { BRIL_CHECK(ip_.block); }

void CFGBuilder::insertInst(Inst *inst) const noexcept {
  checkHasInsertionPoint();

  auto inst_iter = ip_.block->inst_.iterator_to(*ip_.inst);
  ip_.block->inst_.insert(inst_iter, *inst);
  inst->block_ = ip_.block;
}

}  // namespace bril
