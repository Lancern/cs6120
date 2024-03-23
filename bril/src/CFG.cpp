#include "bril/CFG.h"

#include "support/Check.h"

namespace bril {

Block::~Block() noexcept {
  while (!inst_.empty()) {
    auto *i = &inst_.front();
    inst_.pop_front();
    delete i;
  }
}

Function *Block::getFunction() const noexcept {
  if (!cfg_) {
    return nullptr;
  }
  return cfg_->getFunction();
}

CFG::~CFG() noexcept {
  while (!blocks_.empty()) {
    auto *b = &blocks_.front();
    blocks_.pop_front();
    delete b;
  }
}

Block *CFG::getEntryBlock() const noexcept {
  BRIL_CHECK(!blocks_.empty());
  return const_cast<Block *>(&blocks_.front());
}

}  // namespace bril
