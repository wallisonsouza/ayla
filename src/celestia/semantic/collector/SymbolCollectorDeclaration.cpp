#include "celestia/semantic/collector/SymbolCollector.hpp"

#include "celestia/ast/declarations/CapabilityDeclaration.hpp"
#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
namespace celestia::semantic {

void SymbolCollector::collect_generic_parameter(ast::GenericParameter *node) {

  if (!node || !node->name) return;

  auto symbol = declare_symbol(node->name->get_str(), SymbolKind::GenericParameter, Visibility::Private, node);

  if (!symbol.is_valid()) return;

  debug::Trace::log(debug::Category::SymbolCollector, "generic: {} -> {}", node->name->get_str(), symbol.index());
}

void SymbolCollector::collect_struct(ast::StructDeclaration *node) {

  assert(node && node->name);

  auto struct_symbol = declare_symbol(node->name->get_str(), SymbolKind::Struct, node->specifiers.visibility, node);

  if (!struct_symbol.is_valid()) return;

  ScopeId struct_scope = context.env().scopes.create_scope(core::ScopeKind::Struct, context.stack.current());

  if (!struct_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, struct_scope);

  context.stack.push(struct_scope);

  debug::Trace::header(debug::Category::SymbolCollector, "Struct '{}' (scope {})", node->name->get_str(), struct_scope.index());

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  // Fields
  for (auto *field : node->fields) {
    if (field) collect_node(field);
  }

  context.stack.pop();
}

void SymbolCollector::collect_type(ast::TypeDeclaration *node) {

  if (!node || !node->name) return;

  auto type_symbol = declare_symbol(node->name->get_str(), SymbolKind::Type, node->specifiers.visibility, node);

  if (!type_symbol.is_valid()) return;

  ScopeId type_scope = context.env().scopes.create_scope(core::ScopeKind::Type, context.stack.current());

  if (!type_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, type_scope);

  context.stack.push(type_scope);

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  context.stack.pop();
}

void SymbolCollector::collect_variable(ast::VariableDeclaration *node) {

  if (!node) return;

  if (node->pattern) { collect_pattern(node->pattern); }
}

void SymbolCollector::collect_field(ast::FieldDeclaration *node) {

  assert(node && node->name);

  auto field_symbol = declare_symbol(node->name->get_str(), SymbolKind::Field, Visibility::Private, node);

  if (!field_symbol.is_valid()) return;

  // Type is resolved later by the Resolver.
}

void SymbolCollector::collect_capability(ast::CapabilityDeclaration *node) {

  assert(node && node->name);

  auto capability_symbol = declare_symbol(node->name->get_str(), SymbolKind::Capability, node->specifiers.visibility, node);

  if (!capability_symbol.is_valid()) return;

  ScopeId capability_scope = context.env().scopes.create_scope(core::ScopeKind::Capability, context.stack.current());

  if (!capability_scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Capability '{}' (scope {})", node->name->get_str(), capability_scope.index());

  context.unit.semantic.set_scope(node, capability_scope);

  context.stack.push(capability_scope);

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  // Capability functions
  for (auto *member : node->members) {
    if (member) collect_node(member);
  }

  context.stack.pop();
}

void SymbolCollector::collect_impl(ast::ImplDeclaration *node) {

  if (!node) return;

  ScopeId impl_scope = context.env().scopes.create_scope(core::ScopeKind::Impl, context.stack.current());

  if (!impl_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, impl_scope);

  context.stack.push(impl_scope);

  // Implementation members
  for (auto *member : node->members) {
    if (member) collect_node(member);
  }

  context.stack.pop();
}

} // namespace celestia::semantic
