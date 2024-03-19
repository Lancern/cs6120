#pragma once

namespace bril {

[[noreturn]] void checkFailHandler(const char *file, int line, const char *func,
                                   const char *condition);

#define BRIL_CHECK(condition)                                             \
  do {                                                                    \
    if (!(condition)) {                                                   \
      ::bril::checkFailHandler(__FILE__, __LINE__, __func__, #condition); \
    }                                                                     \
  } while (false)

}  // namespace bril
