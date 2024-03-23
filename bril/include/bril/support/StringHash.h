#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>

namespace bril::sup {

/// Transparent string hash.
struct StringHash {
  using is_transparent = void;

  std::size_t operator()(std::string_view s) const noexcept {
    return std::hash<std::string_view>{}(s);
  }

  std::size_t operator()(const std::string &s) const noexcept {
    return std::hash<std::string_view>{}(s);
  }

  std::size_t operator()(const char *s) const noexcept {
    return std::hash<std::string_view>{}(s);
  }
};

} // namespace bril::sup
