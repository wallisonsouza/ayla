#pragma once

#include "celestia/core/visitor/Stage.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

enum class StageId { Lex, ModuleParser, BuiltinModule, Parser, ModuleSetup, Resolver, Check, Lowering, Transpile, SymbolCollector };

inline std::string_view stage_name(StageId id) {
  switch (id) {
    case StageId::Lex:             return "lex";
    case StageId::ModuleParser:    return "module_parser";
    case StageId::BuiltinModule:   return "builtin_module";
    case StageId::Parser:          return "parser";
    case StageId::ModuleSetup:     return "module_setup";
    case StageId::Resolver:        return "resolver";
    case StageId::Check:           return "check";
    case StageId::Lowering:        return "lowering";
    case StageId::Transpile:       return "transpile";
    case StageId::SymbolCollector: return "symbol_collector";
  }

  return "<unknown>";
}

struct StageRule {
  std::vector<StageId> dependencies;
  std::unique_ptr<Stage> stage;
};

class CompilationRules {
public:
  static CompilationRules normal();
  static CompilationRules builtin();
  static CompilationRules discovery();

  template <typename T, typename... Args> void add(StageId id, std::initializer_list<StageId> dependencies, Args &&...args) {

    auto stage = std::make_unique<T>(std::forward<Args>(args)...);

    auto [it, inserted] = stages_.emplace(id, StageRule{.dependencies = std::vector<StageId>(dependencies), .stage = std::move(stage)});

    if (!inserted) { throw std::runtime_error("CompilationRules: duplicate stage"); }
  }

  const StageRule *find(StageId id) const {
    auto it = stages_.find(id);

    if (it == stages_.end()) { return nullptr; }

    return &it->second;
  }

private:
  std::unordered_map<StageId, StageRule> stages_;
};