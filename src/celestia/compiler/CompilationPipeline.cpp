// #include "celestia/compiler/CompilationPipeline.hpp"

// #include <stdexcept>

// void CompilationPipeline::ensure(CompilationUnit &unit, StageId target, const CompilationRules &rules, std::unordered_set<StageId> &completed, std::unordered_set<StageId> &active) {

//   if (completed.contains(target)) { return; }

//   if (!active.insert(target).second) { throw std::runtime_error("CompilationPipeline: cyclic stage dependency: " + std::string(stage_name(target))); }

//   const StageRule *rule = rules.find(target);

//   if (!rule) {
//     active.erase(target);

//     throw std::runtime_error("CompilationPipeline: stage not found: " + std::string(stage_name(target)));
//   }

//   if (!rule->stage) {
//     active.erase(target);

//     throw std::runtime_error("CompilationPipeline: stage is null: " + std::string(stage_name(target)));
//   }

//   for (const auto &dependency : rule->dependencies) { ensure(unit, dependency, rules, completed, active); }

//   if (completed.contains(target)) {
//     active.erase(target);
//     return;
//   }

//   rule->stage->run(compiler_, unit);

//   completed.insert(target);
//   active.erase(target);
// }