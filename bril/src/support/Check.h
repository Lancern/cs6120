#pragma once

namespace bril {

[[noreturn]] void checkFailHandler(const char *file, int line, const char *func,
                                   const char *condition);

[[noreturn]] void unreachableHandler(const char *file, int line, const char *func);

#define BRIL_CHECK(condition)                                             \
  do {                                                                    \
    if (!(condition)) {                                                   \
      ::bril::checkFailHandler(__FILE__, __LINE__, __func__, #condition); \
    }                                                                     \
  } while (false)

#define BRIL_UNREACHABLE() ::bril::unreachableHandler(__FILE__, __LINE__, __func__)

}  // namespace bril
