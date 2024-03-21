#include "Check.h"

#include <cstdlib>
#include <iostream>

namespace bril::sup {

[[noreturn]] void checkFailHandler(const char *file, int line, const char *func,
                                   const char *condition) {
  std::cerr << "CHECK FAILED: " << file << ":" << line << ": in " << func << ":\n"
            << condition << " evaluates to false!" << std::endl;
  std::abort();
}

[[noreturn]] void unreachableHandler(const char *file, int line, const char *func) {
  std::cerr << "UNREACHABLE CODE REACHED: " << file << ":" << line << ": in " << func << std::endl;
  std::abort();
}

}  // namespace bril::sup
