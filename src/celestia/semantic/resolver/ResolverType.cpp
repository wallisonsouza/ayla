#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

#include <cassert>

namespace celestia::semantic {

void Resolver::resolve_named_type(ast::NamedType *node) {

  assert(node && node->name);

  ScopeId scope_id = context.stack.current();

  if (!scope_id.is_valid()) return;

  SymbolId symbol_id = require_symbol(node->name, scope_id);

  if (!symbol_id.is_valid()) return;

  context.unit.semantic.set_symbol(node, symbol_id);
}

void Resolver::resolve_generic_type(ast::GenericType *node) {

  assert(node && node->name);

  ScopeId scope_id = context.stack.current();

  if (!scope_id.is_valid()) return;

  SymbolId symbol_id = require_symbol(node->name, scope_id);

  if (!symbol_id.is_valid()) return;

  context.unit.semantic.set_symbol(node, symbol_id);

  for (auto *argument : node->arguments) {

    assert(argument);

    resolve_node(argument);
  }
}

void Resolver::resolve_function_type(ast::FunctionType *node) {}

} // namespace celestia::semantic
