#include "celestia/ast/NodeCast.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"
#include "celestia/semantic/collector/SymbolCollectorDiagnostics.hpp"

namespace celestia::semantic {

void SymbolCollector::collect_pattern(ast::PatternNode *pattern, SymbolKind kind) {
  assert(pattern);

  switch (pattern->kind) {
  case ast::NodeKind::NamedPattern: collect_named_pattern(ast::as<ast::NamedPattern>(pattern), kind); break;

  default: break;
  }
}


void SymbolCollector::collect_named_pattern(ast::NamedPattern *pattern, SymbolKind kind) {
  assert(pattern && pattern->name);

  auto symbol = declare_symbol(pattern->name, kind, Visibility::Private, pattern);

  if (!symbol.is_valid()) return;

  debug::Trace::header(debug::Category::SymbolCollector, "binding '{}' -> {} (scope {})", pattern->name->get_str(), symbol.index(), context.stack.current().index());
}

} // namespace celestia::semantic