#include "celestia/ast/NodeCast.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"
#include "celestia/semantic/collector/SymbolCollectorDiagnostics.hpp"

namespace celestia::semantic {

void SymbolCollector::collect_pattern(ast::PatternNode *pattern) {

  if (!pattern) return;

  switch (pattern->kind) {

  case ast::NodeKind::NamedPattern: collect_named_pattern(ast::as<ast::NamedPattern>(pattern)); break;

  default: break;
  }
}

void SymbolCollector::collect_named_pattern(ast::NamedPattern *pattern) {

  assert(pattern && pattern->name);

  SymbolId symbol = declare_symbol(pattern->name->str, SymbolKind::Variable, Visibility::Private, pattern);

  if (!symbol.is_valid()) {
    celestia::semantic::collector::report_redeclaration(context, pattern->name->get_str(), pattern->name->slice);
    return;
  };

  debug::Trace::log(debug::Category::SymbolCollector, "param: {} -> {}", pattern->name->get_str(), symbol.index());

  if (!symbol.is_valid()) return;
}

} // namespace celestia::semantic