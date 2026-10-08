#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::check_name_pattern(ast::NamedPattern *pattern, ExpectedType expected) {
  assert(pattern);
  assert(expected.is_valid());

  auto &semantic = context.unit.semantic;

  SymbolId symbol_id = semantic.symbol(pattern);

  if (!symbol_id.is_valid()) {
    error(pattern, "pattern has invalid SymbolId");
    return TypeId::invalid();
  }

  auto &symbol = context.get_env().symbols.get(symbol_id);

  if (pattern->type_annotation) {
    TypeId declared_type = resolve_type(pattern->type_annotation);

    if (!declared_type.is_valid()) {
      error(pattern, "invalid pattern type annotation");
      return TypeId::invalid();
    }

    if (!type_system.is_same_type(declared_type, expected.type)) {
      checker::diagnostics::report_type_mismatch(context, expected.node ? expected.node->slice : pattern->slice, pattern->type_annotation->slice, expected.type, declared_type);

      return TypeId::invalid();
    }
  }

  semantic.set_type(pattern, expected.type);
  symbol.type = expected.type;

  return expected.type;
}

} // namespace celestia::semantic