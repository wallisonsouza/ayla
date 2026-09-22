#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

void Resolver::resolve_function_declaration(ast::FunctionDeclaration *node) {

  assert(node && node->name);

  auto scope = context.unit.semantic.scope(node);

  if (!scope.is_valid()) {
    std::cerr << "function has no scope\n";
    return;
  }

  context.stack.push(scope);

  // // generics
  for (auto *generic : node->generic_parameters) {
    if (generic) { resolve_node(generic); }
  }

  // Params
  for (auto *parameter : node->parameters) {
    if (parameter) { resolve_node(parameter); }
  }

  // Return type

  if (node->return_type) { resolve_node(node->return_type); }

  // Body

  if (node->body) { resolve_node(node->body); }

  context.stack.pop();
}
} // namespace celestia::semantic