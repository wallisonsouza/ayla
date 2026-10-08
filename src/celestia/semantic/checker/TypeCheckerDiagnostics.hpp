
#include "celestia/diagnostic/Diagnostic.hpp"
#include "celestia/semantic/SemanticContext.hpp"

namespace diagnostic {
inline Diagnostic type_mismatch(SourceSlice primary_slice, SourceSlice expected_slice, SourceSlice actual_slice, celestia::semantic::TypeId expected, celestia::semantic::TypeId actual) {

  return {

      .severity = Severity::Error,

      .code = DiagnosticCode::TypeMismatch,

      .primary_slice = primary_slice,

      .arguments = {},

      .labels =
          {

              {

                  .slice = expected_slice,

                  .code = LabelCode::ExpectedType,

                  .arguments = {expected},

              },
              {

                  .slice = actual_slice,

                  .code = LabelCode::FoundType,

                  .arguments = {actual},

              },

          },

  };
}

inline Diagnostic
invalid_binary_operation(SourceSlice primary_slice, SourceSlice lhs_slice, SourceSlice rhs_slice, BinaryOperation operation, celestia::semantic::TypeId lhs, celestia::semantic::TypeId rhs) {

  return {

      .severity = Severity::Error,

      .code = DiagnosticCode::InvalidBinaryOperation,

      .primary_slice = primary_slice,

      .arguments = {OperationTypes{.lhs = lhs, .rhs = rhs}, operation},

      .labels =
          {

              {

                  .slice = lhs_slice,

                  .code = LabelCode::LeftOperand,

                  .arguments = {lhs},

              },

              {

                  .slice = rhs_slice,

                  .code = LabelCode::RightOperand,

                  .arguments = {rhs},

              },

          },

  };
}

inline Diagnostic cannot_infer_type(SourceSlice slice) {

  return {

      .severity = Severity::Error,

      .code = DiagnosticCode::CannotInferType,

      .primary_slice = slice,

      .arguments = {},

      .labels =
          {

              {

                  .slice = slice,

                  .code = LabelCode::Location,

                  .arguments = {},

              },

          },

  };
}

} // namespace diagnostic

namespace celestia::semantic::checker::diagnostics {

inline void report_type_mismatch(semantic::SemanticContext &context, SourceSlice expected_slice, SourceSlice actual_slice, semantic::TypeId expected, semantic::TypeId actual) {

  context.unit.diagnostics.report(diagnostic::type_mismatch(context.diagnostic_slice(), expected_slice, actual_slice, expected, actual));
}

inline void report_invalid_binary_operation(semantic::SemanticContext &context, SourceSlice lhs_slice, SourceSlice rhs_slice, BinaryOperation operation, semantic::TypeId lhs, semantic::TypeId rhs) {

  context.unit.diagnostics.report(diagnostic::invalid_binary_operation(context.diagnostic_slice(), lhs_slice, rhs_slice, operation, lhs, rhs));
}

inline void report_cannot_infer_type(semantic::SemanticContext &context, SourceSlice slice) { context.unit.diagnostics.report(diagnostic::cannot_infer_type(slice)); }

} // namespace celestia::semantic::checker::diagnostics