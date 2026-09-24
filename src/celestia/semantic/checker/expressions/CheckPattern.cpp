#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {



bool TypeChecker::check_name_pattern(ast::NamedPattern *pattern, TypeId expected) {

  assert(pattern);
  assert(expected.is_valid());

  auto &semantic = context.unit.semantic;

  SymbolId symbol_id = semantic.symbol(pattern);

  if (!symbol_id.is_valid()) {
    error(pattern, "pattern has invalid SymbolId");
    return false;
  }

  auto &symbol = context.env().symbols.get(symbol_id);

  if (pattern->type_annotation) {

    TypeId declared_type = infer(pattern->type_annotation);

    if (!declared_type.is_valid()) {
      error(pattern, "invalid pattern type annotation");
      return false;
    }

    if (!is_same_type(declared_type, expected)) return false;
  }

  semantic.set_type(pattern, expected);
  symbol.type = expected;

  return true;
}
} // namespace celestia::semantic