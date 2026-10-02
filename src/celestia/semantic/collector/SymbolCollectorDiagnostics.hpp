
#include "celestia/semantic/collector/SymbolCollectorContext.hpp"

namespace celestia::semantic::collector::diagnostics {

inline void report_redeclaration(SymbolCollectorContext &context, const SymbolId id, SourceSlice slice) {

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::RedefinedSymbol,
      .arguments =
          {
              diagnostic::symbol(id),
          },
      .labels =
          {
              diagnostic::location(slice),
          },
  });
}

} // namespace celestia::semantic::collector::diagnostics