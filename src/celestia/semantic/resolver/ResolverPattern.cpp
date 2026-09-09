#include "Resolver.hpp"

namespace celestia::semantic {

void Resolver::named_pattern(ast::NamedPattern *pattern) {

  if (!pattern) return;

  SymbolId symbol_id = context.unit.semantic.symbol(pattern);

  if (!symbol_id.is_valid()) { return; }

  if (pattern->type_annotation) { resolve_node(pattern->type_annotation); }
}

void Resolver::pattern(ast::PatternNode *pattern) {

  if (!pattern) return;

  switch (pattern->kind) {

  case ast::NodeKind::NamedPattern: named_pattern(static_cast<ast::NamedPattern *>(pattern)); break;

  default: break;
  }
}

} // namespace celestia::semantic