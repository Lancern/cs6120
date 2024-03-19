#include "Check.h"

#include <cstdlib>
#include <iostream>

namespace bril {

[[noreturn]] void checkFailHandler(const char *file, int line, const char *func,
                                   const char *condition) {
  std::cerr << "CHECK FAILED: " << file << ":" << line << ": in " << func << ":\n"
            << condition << " evaluates to false!" << std::endl;
  std::abort();
}

}  // namespace bril
