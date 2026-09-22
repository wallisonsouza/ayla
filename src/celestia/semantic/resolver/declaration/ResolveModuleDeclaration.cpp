#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

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

  debug::Trace::log(debug::Category::Resolver, "module '{}' state: {}{}{}{}{}{}", module.name(),

               module.has_state(ModuleState::Discovered) ? "Discovered " : "",

               module.has_state(ModuleState::Parsed) ? "Parsed " : "",

               module.has_state(ModuleState::Resolved) ? "Resolved " : "",

               module.has_state(ModuleState::Checked) ? "Checked " : "",

               module.has_state(ModuleState::Lowered) ? "Lowered " : "",

               module.has_state(ModuleState::Failed) ? "Failed" : "");
}

} // namespace

void Resolver::resolve_module_init_declaration(ast::ModuleInitDeclaration *node) {

  if (!node || !node->body) return;

  debug::Trace::log(debug::Category::Resolver, "resolving module init");

  debug::Trace::log(debug::Category::Resolver, "module init body = {}", static_cast<void *>(node->body));

  resolve_node(node->body);

  debug::Trace::log(debug::Category::Resolver, "module init body resolved");
}

} // namespace celestia::semantic
