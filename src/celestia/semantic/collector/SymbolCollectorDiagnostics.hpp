
#include "celestia/semantic/collector/SymbolCollectorContext.hpp"

namespace celestia::semantic::collector {

inline static void report_redeclaration(SymbolCollectorContext &context, const std::string &name, SourceSlice slice) {

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::RedefinedSymbol,
      .arguments =
          {
              diagnostic::name(name),
          },
      .labels =
          {
              diagnostic::location(slice),
          },
  });
}

}; // namespace celestia::semantic