#include "celestia/ast/declarations/CapabilityDeclaration.hpp"
#include "celestia/ast/declarations/EnumDeclaration.hpp"
#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"
#include "celestia/semantic/collector/SymbolCollectorDiagnostics.hpp"

namespace celestia::semantic {

void SymbolCollector::collect_function_declaration(ast::FunctionDeclaration *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::Function, node->specifiers.visibility, node);

  if (!symbol.is_valid()) return;

  auto scope = enter_scope(core::ScopeKind::Function, node);

  if (!scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Function '{}' (scope {})", node->name->get_str(), scope.index());

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  for (auto *parameter : node->parameters) {
    if (parameter) collect_node(parameter);
  }

  if (node->body) collect_node(node->body);

  context.stack.pop();
}

void SymbolCollector::collect_field_declaration(ast::FieldDeclaration *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::Field, Visibility::Private, node);

  if (!symbol.is_valid()) return;

  debug::Trace::log(debug::Category::SymbolCollector, "field '{}' -> symbol {}", node->name->get_str(), symbol.index());

  // The field type is resolved later by the Resolver.
}

void SymbolCollector::collect_generic_parameter(ast::GenericParameter *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::GenericParameter, Visibility::Private, node);

  if (!symbol.is_valid()) return;

  debug::Trace::log(debug::Category::SymbolCollector, "GenericParameter '{}' -> symbol {}", node->name->get_str(), symbol.index());
}

void SymbolCollector::collect_struct_declaration(ast::StructDeclaration *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::Struct, node->specifiers.visibility, node);

  if (!symbol.is_valid()) return;

  auto scope = enter_scope(core::ScopeKind::Struct, node);

  if (!scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Struct '{}' (scope {})", node->name->get_str(), scope.index());

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  for (auto *field : node->fields) {
    if (field) collect_node(field);
  }

  context.stack.pop();
}

void SymbolCollector::collect_type_declaration(ast::TypeDeclaration *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::Type, node->specifiers.visibility, node);

  if (!symbol.is_valid()) return;

  auto scope = enter_scope(core::ScopeKind::Type, node);

  if (!scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Type '{}' (scope {})", node->name->get_str(), scope.index());

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  context.stack.pop();
}

void SymbolCollector::collect_variable_declaration(ast::VariableDeclaration *node) {

  assert(node && node->pattern);

  collect_node(node->pattern);
}

void SymbolCollector::collect_enum_declaration(ast::EnumDeclaration *node) {

  assert(node && node->name);

  // The enum symbol belongs to the enclosing scope.
  auto symbol = declare_named_symbol(node->name, SymbolKind::Enum, node->specifiers.visibility, node);

  if (!symbol.is_valid()) return;

  // Enum members live in the enum's own scope.
  auto scope = enter_scope(core::ScopeKind::Enum, node);

  if (!scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Enum '{}' (scope {})", node->name->get_str(), scope.index());

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  for (auto *variant : node->variants) {
    if (variant) collect_node(variant);
  }

  context.stack.pop();
}

void SymbolCollector::collect_enum_variant(ast::EnumVariant *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::EnumVariant, Visibility::Private, node);

  if (!symbol.is_valid()) return;

  debug::Trace::log(debug::Category::SymbolCollector, "variant '{}' -> symbol {}", node->name->get_str(), symbol.index());

  for (auto *field : node->payload) {
    if (field) collect_node(field);
  }
}

void SymbolCollector::collect_capability_declaration(ast::CapabilityDeclaration *node) {

  assert(node && node->name);

  auto symbol = declare_named_symbol(node->name, SymbolKind::Capability, node->specifiers.visibility, node);

  if (!symbol.is_valid()) return;

  auto scope = enter_scope(core::ScopeKind::Capability, node);

  if (!scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Capability '{}' (scope {})", node->name->get_str(), scope.index());

  for (auto *generic : node->generic_parameters) {
    if (generic) collect_node(generic);
  }

  for (auto *member : node->members) {
    if (member) collect_node(member);
  }

  context.stack.pop();
}

void SymbolCollector::collect_impl_declaration(ast::ImplDeclaration *node) {

  assert(node);

  auto scope = enter_scope(core::ScopeKind::Impl, node);

  if (!scope.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "Impl (scope {})", scope.index());

  for (auto *member : node->members) {
    if (member) collect_node(member);
  }

  context.stack.pop();
}

} // namespace celestia::semantic