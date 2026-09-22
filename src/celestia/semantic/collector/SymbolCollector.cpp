#include "celestia/semantic/collector/SymbolCollector.hpp"
#include "celestia/ast/NodeCast.hpp"
#include "celestia/ast/declarations/CapabilityDeclaration.hpp"

#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/semantic/collector/SymbolCollectorDiagnostics.hpp"
namespace celestia::semantic {

SymbolCollector::SymbolCollector(Compiler &compiler, CompilationUnit &unit) : context(compiler, unit) {}

void SymbolCollector::collect() {
  if (!context.unit._root) return;

  collect_node(context.unit._root);
}

SymbolId SymbolCollector::declare_symbol(const std::string &name, SymbolKind kind, Visibility visibility, ast::Node *node) {

  ScopeId scope_id = context.stack.current();

  if (!scope_id.is_valid()) { return SymbolId::invalid(); }

  auto &scope = context.env().scopes.get(scope_id);

  if (scope.symbols.contains(name)) { return SymbolId::invalid(); }

  SymbolId symbol_id = context.env().symbols.create_symbol(name, kind, visibility, false, node);

  if (!symbol_id.is_valid()) { return SymbolId::invalid(); }

  scope.symbols.insert(name, symbol_id);

  if (node) { context.unit.semantic.set_symbol(node, symbol_id); }

  return symbol_id;
}

void SymbolCollector::collect_root(ast::RootNode *node) {
  if (!node) return;

  for (auto *module : node->modules) {
    if (module) collect_node(module);
  }
}

void SymbolCollector::collect_module_init(ast::ModuleInitDeclaration *node) {

  if (!node || !node->body) return;

  for (auto *item : node->body->items) {

    if (!item) continue;
  }

  context.unit.semantic.set_scope(node, context.stack.current());

  collect_node(node->body);
}

void SymbolCollector::collect_node(ast::Node *node) {

  if (!node) return;

  switch (node->kind) {

  case ast::NodeKind::Root: collect_root(ast::as<ast::RootNode>(node)); break;

  case ast::NodeKind::ModuleDeclaration: collect_module(ast::as<ast::ModuleDeclaration>(node)); break;

  case ast::NodeKind::StructDeclaration: collect_struct(ast::as<ast::StructDeclaration>(node)); break;

  case ast::NodeKind::FunctionDeclaration: collect_function(ast::as<ast::FunctionDeclaration>(node)); break;

  case ast::NodeKind::VariableDeclaration: collect_variable(ast::as<ast::VariableDeclaration>(node)); break;

  case ast::NodeKind::BlockStatement: collect_block(ast::as<ast::BlockStatement>(node)); break;

  case ast::NodeKind::ModuleInitDeclaration: collect_module_init(ast::as<ast::ModuleInitDeclaration>(node)); break;

  case ast::NodeKind::TypeDeclaration: collect_type(ast::as<ast::TypeDeclaration>(node)); break;

  case ast::NodeKind::FieldDeclaration: collect_field(ast::as<ast::FieldDeclaration>(node)); break;

  case ast::NodeKind::CapabilityDeclaration: collect_capability(ast::as<ast::CapabilityDeclaration>(node)); break;

  case ast::NodeKind::GenericParameter: collect_generic_parameter(ast::as<ast::GenericParameter>(node)); break;

  case ast::NodeKind::ImplementationDeclaration: collect_impl(ast::as<ast::ImplDeclaration>(node)); break;

  default: break;
  }
}

void SymbolCollector::collect_module(ast::ModuleDeclaration *node) {

  if (!node || !node->name) return;

  const auto module_name = node->name->get_str();

  // Cria ou reutiliza o módulo.
  auto module_id = context.env().modules.register_module(module_name);

  if (!module_id.is_valid()) { return; }

  auto &builtin = context.env().modules.get(context.env().builtin_module);

  auto &module = context.env().modules.get(module_id);

  // O módulo ainda não possui scope.
  ScopeId scope = module.scope_id();

  if (!scope.is_valid()) {

    ScopeId parent = context.stack.current();

    scope = context.env().scopes.create_scope(core::ScopeKind::Module, builtin.scope_id());

    if (!scope.is_valid()) { return; }

    module.set_scope(scope);
  }

  context.unit.semantic.set_module(node, module_id);
  context.unit.semantic.set_scope(node, scope);

  context.stack.push(scope);

  for (auto *declaration : node->declarations) {

    if (!declaration) continue;

    collect_node(declaration);
  }

  context.stack.pop();
}

void SymbolCollector::collect_block(ast::BlockStatement *node) {

  if (!node) return;

  ScopeId parent = context.stack.current();

  ScopeId block_scope = context.env().scopes.create_scope(core::ScopeKind::Block, parent);

  if (!block_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, block_scope);

  context.stack.push(block_scope);

  for (auto *item : node->items) {
    if (item) collect_node(item);
  }

  context.stack.pop();
}



} // namespace celestia::semantic