#pragma once

#include "celestia/compiler/CompilationRules.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/compiler/UnitManager.hpp"
#include "celestia/core/managers/source_manager.hpp"
#include "celestia/debug/ast/AstDumper.hpp"
#include "celestia/diagnostic/diagnostic_debug.hpp"
#include "celestia/ir/IRProgram.hpp"
#include "celestia/language/LanguageDefinition.hpp"
#include <unordered_set>

class Compiler {
public:
  explicit Compiler(celestia::LanguageDefinition &lang) : environment_(), sources() { environment_.language = lang; }
  celestia::ir::IRProgram program;
  CompilationUnit *add_script(const std::string &path) {

    auto *source = sources.create_source(path);

    auto id = environment_.units.create(*source);

    return environment_.units.get(id);
  }

  void require(CompilationUnit &unit, std::string_view target, const CompilationRules &rules) {

    auto &completed = completed_[unit.id];
    std::unordered_set<std::string> active;

    ensure(unit, target, rules, completed, active);
  }

  void require(celestia::semantic::CompilationUnitId id, std::string_view target, const CompilationRules &rules) {

    auto *unit = environment_.units.get(id);

    if (!unit) { throw std::runtime_error("Compiler: compilation unit not found"); }

    require(*unit, target, rules);
  }

  CompilerEnvironment &environment() { return environment_; }

  void show_diagnostics() {
    for (auto &unit : environment_.units.all()) {
      for (auto &diag : unit->diagnostics.all()) { diagnostic::print_diagnostic(diag, unit->source, environment_); }
    }
  }

  void dump() {
    for (auto &unit : environment_.units.all()) {
      celestia::debug::AstDumper dumper(std::cout);

      dumper.dispatch(unit->_root);
    }
  }

private:
  void ensure(CompilationUnit &unit, std::string_view target, const CompilationRules &rules, std::unordered_set<std::string> &completed, std::unordered_set<std::string> &active) {

    std::string name(target);

    if (completed.contains(name)) return;

    if (!active.insert(name).second) { throw std::runtime_error("Compiler: cyclic stage dependency: " + name); }

    const StageRule *rule = rules.find(target);

    if (!rule) {
      active.erase(name);
      throw std::runtime_error("Compiler: stage not found: " + name);
    }

    if (!rule->stage) {
      active.erase(name);
      throw std::runtime_error("Compiler: stage is null: " + name);
    }

    for (const auto &dependency : rule->dependencies) { ensure(unit, dependency, rules, completed, active); }

    if (completed.contains(name)) {
      active.erase(name);
      return;
    }

    rule->stage->run(*this, unit);

    completed.insert(std::move(name));
    active.erase(name);
  }

private:
  CompilerEnvironment environment_;
  SourceManager sources;

  std::unordered_map<celestia::semantic::CompilationUnitId, std::unordered_set<std::string>> completed_;
};