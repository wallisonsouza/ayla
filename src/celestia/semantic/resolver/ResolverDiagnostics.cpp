#include "celestia/semantic/SemanticContext.hpp"

namespace celestia::semantic::collector::diagnostics {

inline void report_unknown_module(SemanticContext &context, std::string name, SourceSlice slice) {

  context.unit.diagnostics.report({

      .severity = diagnostic::Severity::Error,

      .code = diagnostic::DiagnosticCode::UnknownModule,

      .arguments =
          {
              std::move(name),
          },

      .labels =
          {

              {
                  .slice = slice,

                  .kind = diagnostic::LabelKind::Primary,

                  .code = diagnostic::LabelCode::Location,

                  .arguments = {},
              },

          },

  });
}

} // namespace celestia::semantic::collector::diagnostics