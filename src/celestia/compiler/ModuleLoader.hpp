#include "celestia/ast/declarations/ImportDeclaration.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"

bool load_module(Compiler &compiler, const celestia::ast::ImportDeclaration *node, CompilationUnit &importer) {
  if (!node || !node->name) return false;

  auto module_id = compiler.environment().modules.find(node->name->get_str());

  if (!module_id.is_valid()) return false;

  const auto &providers = compiler.environment().index.files(module_id);

  for (const auto &file : providers) {

    auto *unit = compiler.add_script(file.string());

    if (!unit) return false;

    compiler.require(*unit, StageId::Resolver, CompilationRules::normal());
  }

  return true;
}