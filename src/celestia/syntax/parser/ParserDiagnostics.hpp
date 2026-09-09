
#include "celestia/syntax/parser/ParserContext.hpp"

namespace celestia::syntax::parser::diagnostics {

inline void report_expected(ParseContext &context, TokenKind expected) {

  auto &tokens = context.tokens();

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::Expected,
      .arguments =
          {
              diagnostic::found(tokens.current()),
              diagnostic::expected_token(expected),
          },
      .labels =
          {
              diagnostic::location(context.diagnostic_slice()),
          },
  });
}

inline void report_missing_pattern_colon(ParseContext &context) {
  auto &tokens = context.tokens();

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::Expected,

      .arguments =
          {
              diagnostic::found(tokens.current()),
              diagnostic::expected_token(TokenKind::COLON, diagnostic::ExpectedPosition::Before),
          },

      .labels =
          {
              diagnostic::location(context.diagnostic_slice()),
          },

      .helps =
          {
              {
                  .code = diagnostic::HelpCode::AddTypeAnnotation,
                  .arguments = {},
              },
          },
  });
}

inline void report_expected(ParseContext &context, diagnostic::ExpectedKind expected) {

  auto &tokens = context.tokens();

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::Expected,
      .arguments =
          {
              diagnostic::found(tokens.current()),
              diagnostic::expected_category(expected),
          },
      .labels =
          {
              diagnostic::location(context.diagnostic_slice()),
          },
  });
}

inline void report_missing(ParseContext &context, TokenKind expected) {

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::Expected,

      .arguments =
          {
              diagnostic::expected_token(expected),
          },

      .labels =
          {
              diagnostic::location(context.diagnostic_slice()),
          },
  });
}

inline void report_unexpected(ParseContext &context) {

  auto &tokens = context.tokens();

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::Unexpected,
      .arguments =
          {
              diagnostic::found(tokens.current()),
          },
      .labels =
          {
              diagnostic::location(context.diagnostic_slice()),
          },
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

inline void report_missing_return_arrow(ParseContext &context) {

  auto &tokens = context.tokens();

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::Expected,

      .arguments =
          {
              diagnostic::found(tokens.current()),
              diagnostic::expected_token(TokenKind::ARROW, diagnostic::ExpectedPosition::Before),
          },

      .labels =
          {
              diagnostic::location(context.diagnostic_slice()),
          },

      .helps =
          {
              {
                  .code = diagnostic::HelpCode::UseReturnArrow,
                  .arguments = {},
              },
          },
  });
}

} // namespace celestia::syntax::parser::diagnostics