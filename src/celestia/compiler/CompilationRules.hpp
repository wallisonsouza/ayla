#pragma once

#include "celestia/core/visitor/Stage.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace stages {

inline constexpr std::string_view Lex = "lex";
inline constexpr std::string_view ModuleParser = "module_parser";
inline constexpr std::string_view BuiltinModule = "builtin_module";
inline constexpr std::string_view Parser = "parser";
inline constexpr std::string_view ModuleSetup = "module_setup";
inline constexpr std::string_view Resolver = "resolver";
inline constexpr std::string_view Check = "check";
inline constexpr std::string_view Lowering = "lowering";
inline constexpr std::string_view Transpile = "transpile";
inline constexpr std::string_view SymbolCollector = "symbol_collector";
} // namespace stages

struct StageRule {
  std::vector<std::string> dependencies;
  std::unique_ptr<Stage> stage;
};

class CompilationRules {
public:

 static CompilationRules normal();
  static CompilationRules builtin();
  static CompilationRules  discovery();

  template <typename T, typename... Args> void add(std::string_view name, std::initializer_list<std::string_view> dependencies, Args &&...args) {

    std::vector<std::string> deps;
    deps.reserve(dependencies.size());

    for (auto dependency : dependencies) { deps.emplace_back(dependency); }

    auto stage = std::make_unique<T>(std::forward<Args>(args)...);

    auto [it, inserted] = stages_.emplace(std::string(name), StageRule{std::move(deps), std::move(stage)});

    if (!inserted) { throw std::runtime_error("CompilationRules: duplicate stage"); }
  }

  const StageRule *find(std::string_view name) const {
    auto it = stages_.find(std::string(name));

    if (it == stages_.end()) return nullptr;

    return &it->second;
  }


private:
  std::unordered_map<std::string, StageRule> stages_;
};