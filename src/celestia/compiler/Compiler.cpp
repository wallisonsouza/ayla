#include "celestia/compiler/Compiler.hpp"

#include "celestia/debug/ast/AstDumper.hpp"
#include "celestia/diagnostic/diagnostic_debug.hpp"

#include <stdexcept>

Compiler::Compiler(celestia::LanguageDefinition &lang) : environment_(), pipeline_(*this) { environment_.language = lang; }

CompilationUnit *Compiler::add_script(const std::string &path) {

  auto normalized = std::filesystem::absolute(path).lexically_normal();

  const auto key = normalized.string();

  if (auto it = units_by_path_.find(key); it != units_by_path_.end()) { return environment_.units.get(it->second); }

  auto *source = environment_.sources.get_or_create(key);

  if (!source) { return nullptr; }

  auto id = environment_.units.create(*source);

  auto *unit = environment_.units.get(id);

  if (!unit) { return nullptr; }

  units_by_path_.emplace(key, id);

  return unit;
}

void Compiler::require(CompilationUnit &unit, StageId target, const CompilationRules &rules) { pipeline_.require(unit, target, rules); }

void Compiler::require(celestia::semantic::CompilationUnitId id, StageId target, const CompilationRules &rules) {

  auto *unit = environment_.units.get(id);

  if (!unit) { throw std::runtime_error("Compiler: compilation unit not found"); }

  require(*unit, target, rules);
}

CompilerEnvironment &Compiler::environment() { return environment_; }

// void Compiler::show_diagnostics() {

//   for (auto &unit : environment_.units.all()) {

//     for (auto &diag : unit->diagnostics.all()) { diagnostic::print_diagnostic(diag, unit->source, environment_); }
//   }
// }

// void Compiler::dump() {

//   for (auto &unit : environment_.units.all()) {

//     celestia::debug::AstDumper dumper(std::cout);

//     dumper.dispatch(unit->_root);
//   }
// }