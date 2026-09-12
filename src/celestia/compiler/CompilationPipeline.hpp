#pragma once

#include "celestia/compiler/CompilationRules.hpp"
#include "celestia/compiler/CompilationUnit.hpp"

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

class Compiler;

class CompilationPipeline {
public:
  explicit CompilationPipeline(Compiler &compiler) : compiler_(compiler) {}

  void require(CompilationUnit &unit, StageId target, const CompilationRules &rules) {

    if (!active_units_.insert(unit.id).second) { throw std::runtime_error("CompilationPipeline: cyclic module dependency"); }

    ensure(unit, target, rules);

    active_units_.erase(unit.id);
  }

private:
  void ensure(CompilationUnit &unit, StageId target, const CompilationRules &rules) {

    auto &completed = completed_[unit.id];
    auto &active = active_stages_[unit.id];

    if (completed.contains(target)) { return; }

    if (!active.insert(target).second) { throw std::runtime_error("CompilationPipeline: cyclic stage dependency: " + std::string(stage_name(target))); }

    const StageRule *rule = rules.find(target);

    if (!rule) {
      active.erase(target);

      throw std::runtime_error("CompilationPipeline: stage not found: " + std::string(stage_name(target)));
    }

    if (!rule->stage) {
      active.erase(target);

      throw std::runtime_error("CompilationPipeline: stage is null: " + std::string(stage_name(target)));
    }

    for (StageId dependency : rule->dependencies) { ensure(unit, dependency, rules); }

    if (!completed.contains(target)) {
      rule->stage->run(compiler_, unit);
      completed.insert(target);
    }

    active.erase(target);
  }

private:
  Compiler &compiler_;

  std::unordered_set<celestia::semantic::CompilationUnitId> active_units_;

  std::unordered_map<celestia::semantic::CompilationUnitId, std::unordered_set<StageId>> completed_;

  std::unordered_map<celestia::semantic::CompilationUnitId, std::unordered_set<StageId>> active_stages_;
};