#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"
#include "celestia/semantic/collector/SymbolCollectorDiagnostics.hpp"

namespace celestia::semantic {

void SymbolCollector::collect_named_pattern(ast::NamedPattern *pattern) {

  assert(pattern && pattern->name);

  SymbolId symbol = declare_symbol(pattern->name->str, SymbolKind::Variable, Visibility::Private, pattern);

  auto scope = context.stack.current();

  if (!symbol.is_valid()) {
    collector::diagnostics::report_redeclaration(context, pattern->name->get_str(), pattern->name->slice);
    return;
  };
  debug::Trace::header(debug::Category::SymbolCollector, "param: {} -> {} scope {}", pattern->name->get_str(), symbol.index(), scope.index());
  // debug::Trace::log(debug::Category::SymbolCollector, "param: {} -> {}", pattern->name->get_str(), symbol.index());

  if (!symbol.is_valid()) return;
}

} // namespace celestia::semantic