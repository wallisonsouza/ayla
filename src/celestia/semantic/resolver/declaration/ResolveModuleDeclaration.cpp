#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

#include <stdexcept>

namespace celestia::semantic {

namespace {

const char *module_state_name(ModuleState state) {

  switch (state) {

  case ModuleState::None: return "None";

  case ModuleState::Discovered: return "Discovered";

  case ModuleState::Parsed: return "Parsed";

  case ModuleState::Resolved: return "Resolved";

  case ModuleState::Checked: return "Checked";

  case ModuleState::Lowered: return "Lowered";

  case ModuleState::Failed: return "Failed";
  }

  return "Unknown";
}

void log_module_state(Module &module) {

  debug::trace(debug::Category::Resolver, "module '{}' state: {}{}{}{}{}{}", module.name(),

               module.has_state(ModuleState::Discovered) ? "Discovered " : "",

               module.has_state(ModuleState::Parsed) ? "Parsed " : "",

               module.has_state(ModuleState::Resolved) ? "Resolved " : "",

               module.has_state(ModuleState::Checked) ? "Checked " : "",

               module.has_state(ModuleState::Lowered) ? "Lowered " : "",

               module.has_state(ModuleState::Failed) ? "Failed" : "");
}

} // namespace

void Resolver::resolve_module_declaration(ast::ModuleDeclaration *node) {

  if (!node) return;

  const auto module_id = context.unit.semantic.module(node);

  if (!module_id.is_valid()) { throw std::runtime_error("Resolver: module not initialized: " + node->name->get_str()); }

  auto &module = context.get_env().modules.get(module_id);

  debug::trace(debug::Category::Resolver, "resolving module '{}'", module.name());

  if (module.has_state(ModuleState::Resolved)) {
    debug::trace(debug::Category::Resolver, "module '{}' already resolved", module.name());

    return;
  }

  // --------------------------------------------------
  // Imports
  // --------------------------------------------------

  debug::trace(debug::Category::Resolver, "registering imports for '{}'", module.name());

  for (auto *declaration : node->declarations) {

    if (!declaration) continue;

    if (declaration->kind != ast::NodeKind::ImportDeclaration) continue;

    resolve_node(declaration);
  }

  // --------------------------------------------------
  // Dependencies
  // --------------------------------------------------

  for (auto imported_id : module.imports()) {

    if (!imported_id.is_valid()) continue;

    // Builtin ignore.
    if (imported_id == context.get_env().builtin_module) continue;

    auto &imported = context.get_env().modules.get(imported_id);

    debug::trace(debug::Category::Resolver, "module '{}' depends on '{}'", module.name(), imported.name());

    const auto &providers = context.get_env().modules.providers(imported_id);

    if (providers.empty()) { throw std::runtime_error("[resolver]: module has no compilation unit: " + std::string(imported.name())); }

    for (auto unit_id : providers) {

      auto *unit = context.get_env().units.get(unit_id);

      if (!unit) { throw std::runtime_error("[resolver]: compilation unit not found for module: " + std::string(imported.name())); }

      context.compiler.require(*unit, stages::Resolver, CompilationRules::normal());
    }

    debug::trace(debug::Category::Resolver, "dependency '{}' resolved", imported.name());
  }
  // --------------------------------------------------
  // Declarations
  // --------------------------------------------------

  debug::trace(debug::Category::Resolver, "resolving declarations of '{}'", module.name());

  for (auto *declaration : node->declarations) {

    if (!declaration) continue;

    if (declaration->kind == ast::NodeKind::ImportDeclaration) continue;

    resolve_node(declaration);
  }

  module.add_state(ModuleState::Resolved);

  debug::trace(debug::Category::Resolver, "module '{}' resolved", module.name());

  log_module_state(module);
}

void Resolver::resolve_module_init_declaration(ast::ModuleInitDeclaration *node) {

  if (!node || !node->body) return;

  debug::trace(debug::Category::Resolver, "resolving module init");

  debug::trace(debug::Category::Resolver, "module init body = {}", static_cast<void *>(node->body));

  resolve_node(node->body);

  debug::trace(debug::Category::Resolver, "module init body resolved");
}

} // namespace celestia::semantic
