#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

TypeId TypeChecker::check_named_type(ast::NamedType *node) {

  if (!node) return TypeId::invalid();

  SymbolId symbol_id = context.unit.semantic.symbol(node);

  if (!symbol_id.is_valid()) {

    error(node, "named type symbol not found");
    return TypeId::invalid();
  }

  auto &symbol = context.env().symbols.get(symbol_id);

  if (!symbol.type.is_valid()) {

    error(node, "symbol has no valid type");
    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, symbol.type);

  return symbol.type;
}

} // namespace celestia::semantic