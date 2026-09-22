#pragma once

#include <format>
#include <iostream>
#include <string_view>
#include <utility>

namespace celestia::debug {

enum class Category : uint32_t {
  None = 0,
  Lexer = 1u << 0,
  Parser = 1u << 1,
  Resolver = 1u << 2,
  TypeChecker = 1u << 3,
  SymbolCollector = 1u << 4,
  Lowering = 1u << 5,
  IR = 1u << 6,
  Codegen = 1u << 7,

  All = Lexer | Parser | Resolver | TypeChecker | SymbolCollector | Lowering | IR | Codegen
};

constexpr Category operator|(Category a, Category b) { return static_cast<Category>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); }

constexpr Category operator&(Category a, Category b) { return static_cast<Category>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b)); }

constexpr Category operator~(Category value) { return static_cast<Category>(~static_cast<uint32_t>(value)); }

constexpr std::string_view category_name(Category category) {

  switch (category) {
  case Category::Lexer: return "Lexer";
  case Category::Parser: return "Parser";
  case Category::Resolver: return "Resolver";
  case Category::TypeChecker: return "TypeChecker";
  case Category::Lowering: return "Lowering";
  case Category::IR: return "IR";
  case Category::Codegen: return "Codegen";
  case Category::None:
  case Category::SymbolCollector: return "SymbolCollector";
  case Category::All: break;
  }

  return "Unknown";
}

class Trace {
public:
  static void enable(Category categories) { mask() = mask() | categories; }

  static void disable(Category categories) { mask() = mask() & ~categories; }

  static bool is_enabled(Category category) { return (mask() & category) != Category::None; }

  template <typename... Args> static void header(Category category, std::format_string<Args...> format, Args &&...args) {

    if (!is_enabled(category)) return;

    std::cout << "\n" << '[' << category_name(category) << "] " << std::format(format, std::forward<Args>(args)...) << '\n';
  }

  template <typename... Args> static void log(Category category, std::format_string<Args...> format, Args &&...args) {

    if (!is_enabled(category)) return;

    std::cout << '[' << category_name(category) << "]     " << std::format(format, std::forward<Args>(args)...) << '\n';
  }

private:
  static Category &mask() {
    static Category value = Category::None;
    return value;
  }
};
} // namespace celestia::debug