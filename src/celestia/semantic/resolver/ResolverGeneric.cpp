#include "celestia/ast/names/Generic.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
namespace celestia::semantic {

void Resolver::resolve_generic_parameter(ast::GenericParameter *node) {

  if (!node || !node->name) return;

  SymbolId symbol_id = context.unit.semantic.symbol(node);

  if (!symbol_id.is_valid()) { return; }

  // Resolve its constraints.
  for (auto *constraint : node->constraints) {
    if (constraint) { resolve_node(constraint); }
  }
}
} // namespace celestia::semantic