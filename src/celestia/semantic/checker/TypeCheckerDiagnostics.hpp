
#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic::checker::diagnostics {

inline void report_type_mismatch(semantic::TypeCheckerContext &context, SourceSlice slice, semantic::TypeId expected, semantic::TypeId found) {

  context.unit.diagnostics.report({

      .severity = diagnostic::Severity::Error,

      .code = diagnostic::DiagnosticCode::TypeMismatch,

      .arguments =
          {
              diagnostic::name(context.env().types.get(expected).to_string()),

              diagnostic::found(context.env().types.get(found).to_string()),
          },

      .labels =
          {
              diagnostic::location(slice),
          },

  });
}

inline void report_cannot_infer_type(semantic::TypeCheckerContext &context, SourceSlice slice) {

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::CannotInferType,
      .arguments = {
       
      },
      .labels =
          {
              diagnostic::location(slice),
          },
  });
}

} // namespace celestia::semantic::checker::diagnostics