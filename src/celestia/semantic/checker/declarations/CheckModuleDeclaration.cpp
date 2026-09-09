#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {
#include "celestia/compiler/CompilationRules.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"

void TypeChecker::check_module_declaration(ast::ModuleDeclaration *node) {
  if (!node) return;

  auto module = context.unit.semantic.module(node);

  if (module.is_valid()) std::cout << "[TypeChecker] checking module: " << context.env().modules.get(module).name() << '\n';

  // auto module_id = context.unit.module;

  // if (!module_id.is_valid()) {
  //   error(node, "module has invalid ModuleId");
  //   return;
  // }

  // auto &module = context.env().modules.get(module_id);

  // for (auto imported_id : module.imports()) {

  //   if (!imported_id.is_valid()) continue;

  //   auto &dependency = context.env().modules.get(imported_id);

  //   if (dependency.has_state(ModuleState::Checked)) continue;

  //   if (!dependency.unit_.is_valid()) {
  //     error(node, "module dependency has no compilation unit: " + std::string(dependency.name()));
  //     return;
  //   }

  //   auto *unit = context.compiler.environment().units.get(dependency.unit_);

  //   if (!unit) {
  //     error(node, "compilation unit not found for module: " + std::string(dependency.name()));
  //     return;
  //   }

  //   context.compiler.require(*unit, stages::Check, CompilationRules::normal());
  // }

  // --------------------------------------------------
  // Agora verifica as declarações deste módulo
  // --------------------------------------------------

  for (auto *declaration : node->declarations) {

    if (!declaration) continue;

    check(declaration);
  }

  // module.add_state(ModuleState::Checked);

  std::cout << "[TypeChecker] module checked\n";
}

void TypeChecker::check_module_init_declaration(ast::ModuleInitDeclaration *node) {
  if (!node) return;

  std::cout << "[TypeChecker] checking module init\n";

  if (!node->body) return;

  check(node->body);
}
} // namespace celestia::semantic