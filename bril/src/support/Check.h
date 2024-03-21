#pragma once

namespace bril::sup {

[[noreturn]] void checkFailHandler(const char *file, int line, const char *func,
                                   const char *condition);

[[noreturn]] void unreachableHandler(const char *file, int line, const char *func);

#define BRIL_CHECK(condition)                                                  \
  do {                                                                         \
    if (!(condition)) {                                                        \
      ::bril::sup::checkFailHandler(__FILE__, __LINE__, __func__, #condition); \
    }                                                                          \
  } while (false)

#define BRIL_UNREACHABLE() ::bril::sup::unreachableHandler(__FILE__, __LINE__, __func__)

}  // namespace bril::sup
