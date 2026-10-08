#include "celestia/compiler/ModuleLoader.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

#include "celestia/ast/declaration/CapabilityDeclaration.hpp"
#include "celestia/ast/declaration/EnumDeclaration.hpp"
#include "celestia/ast/declaration/FunctionDeclaration.hpp"
#include "celestia/ast/declaration/ImplementationDeclaration.hpp"
#include "celestia/ast/declaration/StructDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"
namespace celestia::semantic {

// type
void Resolver::resolve_type_declaration(ast::TypeDeclaration *node) {}
void Resolver::resolve_capability_declaration(ast::CapabilityDeclaration *node) {}
void Resolver::resolve_impl_declaration(ast::ImplDeclaration *node) {}

void Resolver::resolve_import_declaration(ast::ImportDeclaration *node) {
  assert(node && node->name);

  if (!load_module(context.compiler, node, context.unit)) { return; }
}

void Resolver::resolve_module_declaration(ast::ModuleDeclaration *node) {
  assert(node && node->name);

  auto scope = context.unit.semantic.scope(node);

  stack.push(scope);

  for (auto *declaration : node->declarations) {
    if (declaration) resolve_node(declaration);
  }

  stack.pop();
}

void Resolver::resolve_enum_declaration(ast::EnumDeclaration *node) {
  assert(node && node->name);

  auto scope = context.unit.semantic.scope(node);

  stack.push(scope);

  for (auto *generic : node->generic_parameters) {
    if (generic) resolve_node(generic);
  }

  for (auto *variant : node->variants) {
    if (variant) resolve_node(variant);
  }

  stack.pop();
}

void Resolver::resolve_enum_variant(ast::EnumVariant *node) {
  assert(node && node->name);

  for (auto *field : node->payload) {
    if (field) resolve_node(field);
  }
}

void Resolver::resolve_function_declaration(ast::FunctionDeclaration *node) {
  assert(node && node->name);

  auto scope = context.unit.semantic.scope(node);

  stack.push(scope);

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

  stack.pop();
}

void Resolver::resolve_field_declaration(ast::FieldDeclaration *node) {
  assert(node && node->name);

  if (node->type) { resolve_node(node->type); }
}

void Resolver::resolve_struct_declaration(ast::StructDeclaration *node) {
  assert(node && node->name);

  auto scope = context.unit.semantic.scope(node);

  stack.push(scope);

  // generics
  for (auto *generic : node->generic_parameters) {
    if (generic) resolve_node(generic);
  }

  // compositions
  for (auto *composition : node->compositions) {
    if (composition) resolve_node(composition);
  }

  // fields
  for (auto *field : node->fields) {
    if (field) resolve_node(field);
  }

  stack.pop();
}

void Resolver::resolve_variable_declaration(ast::VariableDeclaration *node) {
  assert(node && node->pattern);

  if (node->pattern) resolve_node(node->pattern);

  if (node->initializer) resolve_node(node->initializer);
}

} // namespace celestia::semantic