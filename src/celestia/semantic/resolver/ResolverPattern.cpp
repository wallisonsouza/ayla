#include "Resolver.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"

namespace celestia::semantic {

void Resolver::named_pattern(ast::NamedPattern *pattern) {

  if (!pattern) return;

  SymbolId symbol_id = context.unit.semantic.symbol(pattern);

  if (!symbol_id.is_valid()) { return; }

  if (pattern->type_annotation) { resolve_node(pattern->type_annotation); }
}

} // namespace celestia::semantic