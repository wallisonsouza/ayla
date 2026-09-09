#include "celestia/semantic/collector/SymbolCollector.hpp"

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

  celestia::debug::trace(celestia::debug::Category::Resolver, "symbol '{}' declared: symbol={}, scope={}", name, symbol_id.index(), scope_id.index());

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

  std::cout << "[SymbolCollector] module init statements = " << node->body->statements.size() << '\n';

  for (auto *statement : node->body->statements) {

    if (!statement) continue;

    std::cout << "[SymbolCollector] init statement kind = " << static_cast<int>(statement->kind) << '\n';
  }

  context.unit.semantic.set_scope(node, context.stack.current());

  collect_node(node->body);
}

void SymbolCollector::collect_node(ast::Node *node) {

  if (!node) return;

  switch (node->kind) {

  case ast::NodeKind::Root: collect_root(static_cast<ast::RootNode *>(node)); break;

  case ast::NodeKind::ModuleDeclaration: collect_module(static_cast<ast::ModuleDeclaration *>(node)); break;

  case ast::NodeKind::StructDeclaration: collect_struct(static_cast<ast::StructDeclaration *>(node)); break;

  case ast::NodeKind::FunctionDeclaration: collect_function(static_cast<ast::FunctionDeclaration *>(node)); break;

  case ast::NodeKind::VariableDeclaration: collect_variable(static_cast<ast::VariableDeclaration *>(node)); break;

  case ast::NodeKind::BlockStatement: collect_block(static_cast<ast::BlockStatement *>(node)); break;

  case ast::NodeKind::ModuleInitDeclaration: collect_module_init(static_cast<ast::ModuleInitDeclaration *>(node)); break;

  case ast::NodeKind::TypeDeclaration: collect_type(static_cast<ast::TypeDeclaration *>(node)); break;

  case ast::NodeKind::FieldDeclaration: collect_field(static_cast<ast::FieldDeclaration *>(node)); break;

  default: break;
  }
}

void SymbolCollector::collect_module(ast::ModuleDeclaration *node) {

  if (!node) return;

  // O ParserStage pode ter associado explicitamente este módulo,
  // como acontece com o módulo builtin.
  auto module_id = context.unit.semantic.module(node);

  // Módulos normais são encontrados pelo nome descoberto.
  if (!module_id.is_valid()) {

    if (!node->name) return;

    module_id = context.env().modules.find(node->name->get_str());

    if (!module_id.is_valid()) return;
  }

  ScopeId parent = context.stack.current();

  ScopeId scope = context.env().scopes.create_scope(core::ScopeKind::Module, parent);

  if (!scope.is_valid()) return;

  context.stack.push(scope);

  context.unit.semantic.set_scope(node, scope);
  context.unit.semantic.set_module(node, module_id);

  auto &module = context.env().modules.get(module_id);

  module.set_scope(scope);

  for (auto *declaration : node->declarations) {

    std::cout << "[SymbolCollector] declaration kind = " << ast::node_kind_name(declaration->kind) << '\n';
    collect_node(declaration);
  }

  context.stack.pop();
}

void SymbolCollector::collect_pattern(ast::PatternNode *pattern) {

  if (!pattern) return;

  switch (pattern->kind) {

  case ast::NodeKind::NamedPattern: collect_named_pattern(static_cast<ast::NamedPattern *>(pattern)); break;

  default: break;
  }
}

void SymbolCollector::collect_block(ast::BlockStatement *node) {

  if (!node) return;

  ScopeId parent = context.stack.current();

  ScopeId block_scope = context.env().scopes.create_scope(core::ScopeKind::Block, parent);

  if (!block_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, block_scope);

  context.stack.push(block_scope);

  for (auto *statement : node->statements) {
    if (statement) collect_node(statement);
  }

  context.stack.pop();
}

void SymbolCollector::collect_generics(const std::vector<ast::IdentifierNode *> &parameters) {

  for (auto *parameter : parameters) {

    if (!parameter) continue;

    declare_symbol(parameter->str, SymbolKind::Type, Visibility::Private, parameter);
  }
}

void SymbolCollector::collect_function(ast::FunctionDeclaration *node) {

  if (!node || !node->name) return;

  const std::string &name = node->name->str;

  // Símbolo da função pertence ao scope atual
  SymbolId function_symbol = declare_symbol(name, SymbolKind::Function, node->specifiers.visibility, node);

  if (!function_symbol.is_valid()) return;

  // Scope da função
  ScopeId function_scope = context.env().scopes.create_scope(core::ScopeKind::Function, context.stack.current());

  if (!function_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, function_scope);

  context.stack.push(function_scope);

  // Generics da função
  collect_generics(node->generic_parameters);

  // Parâmetros
  for (auto *parameter : node->parameters) {

    if (!parameter) continue;

    collect_pattern(parameter);
  }

  // Corpo
  if (node->body) { collect_node(node->body); }

  context.stack.pop();
}

void SymbolCollector::collect_struct(ast::StructDeclaration *node) {

  std::cout << "[SymbolCollector]: colleting struct declaration" << std::endl;

  if (!node || !node->name) return;

  SymbolId symbol_id = declare_symbol(node->name->get_str(), SymbolKind::Type, node->specifiers.visibility, node);

  if (!symbol_id.is_valid()) return;

  ScopeId struct_scope = context.env().scopes.create_scope(core::ScopeKind::Struct, context.stack.current());

  if (!struct_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, struct_scope);

  context.stack.push(struct_scope);

  // struct<T1, T2, ...>
  collect_generics(node->generic_parameters);

  // Campos
  for (auto *field : node->fields) {
    if (field) collect_node(field);
  }

  context.stack.pop();
}

void SymbolCollector::collect_type(ast::TypeDeclaration *node) {

  if (!node || !node->name) return;

  SymbolId symbol_id = declare_symbol(node->name->str, SymbolKind::Type, node->specifiers.visibility, node);

  if (!symbol_id.is_valid()) return;

  ScopeId type_scope = context.env().scopes.create_scope(core::ScopeKind::Type, context.stack.current());

  if (!type_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, type_scope);

  context.stack.push(type_scope);

  collect_generics(node->generic_parameters);

  context.stack.pop();
}

void SymbolCollector::collect_variable(ast::VariableDeclaration *node) {

  if (!node) return;

  if (node->pattern) { collect_pattern(node->pattern); }
}

void SymbolCollector::collect_named_pattern(ast::NamedPattern *pattern) {

  assert(pattern && pattern->name);

  SymbolId symbol_id = declare_symbol(pattern->name->str, SymbolKind::Variable, Visibility::Private, pattern);

  if (!symbol_id.is_valid()) return;
}

void SymbolCollector::collect_field(ast::FieldDeclaration *node) {

  if (!node || !node->name) return;

  SymbolId symbol_id = declare_symbol(node->name->str, SymbolKind::Field, Visibility::Private, node);

  if (!symbol_id.is_valid()) return;

  // O tipo é resolvido depois pelo Resolver.
}

} // namespace celestia::semantic