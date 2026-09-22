#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"

namespace celestia::semantic {

void SymbolCollector::collect_function(ast::FunctionDeclaration *node) {

  assert(node && node->name);

  auto function_symbol = declare_symbol(node->name->get_str(), SymbolKind::Function, node->specifiers.visibility, node);
  if (!function_symbol.is_valid()) return;

  auto parent_scope = context.stack.current();

  ScopeId function_scope = context.env().scopes.create_scope(core::ScopeKind::Function, parent_scope);
  if (!function_scope.is_valid()) return;

  context.unit.semantic.set_scope(node, function_scope);
  context.stack.push(function_scope);

  debug::Trace::header(debug::Category::SymbolCollector, "Function '{}' (scope {}) parent: (scope {})", node->name->get_str(), function_scope.index(), parent_scope.index());

  for (auto *generic : node->generic_parameters) {
    if (!generic) continue;
    collect_node(generic);
  }

  // Parameters
  for (auto *parameter : node->parameters) {
    if (!parameter) continue;

    collect_pattern(parameter);
  }

  // Body
  if (node->body) { collect_node(node->body); }

  context.stack.pop();
}
} // namespace celestia::semantic