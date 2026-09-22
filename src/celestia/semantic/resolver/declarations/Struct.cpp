#include "celestia/compiler/ModuleLoader.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
namespace celestia::semantic {

void Resolver::resolve_field_declaration(ast::FieldDeclaration *node) {

  if (!node) return;

  if (node->type) { resolve_node(node->type); }
}

void Resolver::resolve_struct_declaration(ast::StructDeclaration *node) {

  if (!node) return;

  auto scope = context.unit.semantic.scope(node);

  if (!scope.is_valid()) { std::cerr << "struct has no scope"; }

  context.stack.push(scope);

  for (auto *composition : node->compositions) {
    if (composition) resolve_node(composition);
  }

  for (auto *field : node->fields) {
    if (field) resolve_node(field);
  }

  context.stack.pop();
}

} // namespace celestia::semantic