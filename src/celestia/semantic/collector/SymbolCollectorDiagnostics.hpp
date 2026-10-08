
#include "celestia/semantic/SemanticContext.hpp"

namespace celestia::semantic::collector::diagnostics {

inline void report_redeclaration(SemanticContext &context, SymbolId id, SourceSlice slice) {

  context.unit.diagnostics.report({

      .severity = diagnostic::Severity::Error,

      .code = diagnostic::DiagnosticCode::RedefinedSymbol,

      .arguments =
          {
              id,
          },

      .labels =
          {
              diagnostic::label(slice, diagnostic::LabelCode::ConflictingDeclaration),
          },

  });
}

} // namespace celestia::semantic::collector::diagnostics
