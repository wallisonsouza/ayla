#include "celestia/compiler/ModuleLoader.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

// var
void Resolver::resolve_variable_declaration(ast::VariableDeclaration *node) {

  if (!node) return;

  if (node->pattern) resolve_node(node->pattern);

  if (node->initializer) resolve_node(node->initializer);
}

} // namespace celestia::semantic