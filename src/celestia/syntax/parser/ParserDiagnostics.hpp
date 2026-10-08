#include "celestia/diagnostic/Diagnostic.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

namespace celestia::syntax::parser::diagnostics {

inline void report(ParseContext &context, diagnostic::DiagnosticCode code, std::vector<diagnostic::Argument> arguments) {

  context.unit.diagnostics.report({

      .severity = diagnostic::Severity::Error,

      .code = code,

      .arguments = std::move(arguments),

      .labels =
          {

              {

                  .slice = context.diagnostic_slice(),

                  .code = diagnostic::LabelCode::ExpectedHere,

                  .arguments = {},

              },

          },

  });
}

//--------------------------------------------------
// Expected
//--------------------------------------------------

inline void report_expected(ParseContext &context, TokenKind expected) {

  auto &tokens = context.tokens();

  report(context, diagnostic::DiagnosticCode::Expected,
         {

             diagnostic::ExpectedToken{
                 .kind = expected,
                 .position = diagnostic::ExpectedPosition::Before,
             },

             tokens.current(),

         });
}

inline void report_expected(ParseContext &context, diagnostic::ExpectedKind expected) {

  auto &tokens = context.tokens();

  report(context, diagnostic::DiagnosticCode::Expected,
         {

             diagnostic::ExpectedCategory{
                 .kind = expected,
                 .position = diagnostic::ExpectedPosition::Before,
             },

             tokens.current(),

         });
}

inline void report_missing(ParseContext &context, TokenKind expected) {

  report(context, diagnostic::DiagnosticCode::Expected,
         {

             diagnostic::ExpectedToken{
                 .kind = expected,
                 .position = diagnostic::ExpectedPosition::Before,
             },

         });
}

inline void report_unexpected(ParseContext &context) {

  auto &tokens = context.tokens();

  report(context, diagnostic::DiagnosticCode::Unexpected,
         {

             tokens.current(),

         });
}

//--------------------------------------------------
// Expected
//--------------------------------------------------

inline void report_expected_identifier(ParseContext &context) { report_expected(context, diagnostic::ExpectedKind::Identifier); }

inline void report_expected_pattern(ParseContext &context) { report_expected(context, diagnostic::ExpectedKind::Pattern); }

inline void report_expected_expression(ParseContext &context) { report_expected(context, diagnostic::ExpectedKind::Expression); }

inline void report_expected_statement(ParseContext &context) { report_expected(context, diagnostic::ExpectedKind::Statement); }

inline void report_expected_type(ParseContext &context) { report_expected(context, diagnostic::ExpectedKind::Type); }

//--------------------------------------------------
// Specific diagnostics
//--------------------------------------------------

inline void report_missing_pattern_colon(ParseContext &context) { report_expected(context, TokenKind::COLON); }

inline void report_missing_return_arrow(ParseContext &context) { report_expected(context, TokenKind::ARROW); }

} // namespace celestia::syntax::parser::diagnostics