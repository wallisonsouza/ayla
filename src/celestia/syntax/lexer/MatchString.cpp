#include "Lexer.hpp"

Token *Lexer::match_string() {
  char32_t quote = stream.peek();

  if (quote != U'"' && quote != U'\'') { return nullptr; }

  // consome '"'
  stream.advance();

  auto start = stream.get_state();

  bool escaped = false;
  bool closed = false;

  while (!stream.eof()) {
    char32_t ch = stream.peek();

    if (escaped) {
      escaped = false;
      stream.advance();
      continue;
    }

    if (ch == U'\\') {
      escaped = true;
      stream.advance();
      continue;
    }

    if (ch == quote) {
      closed = true;
      break;
    }

    stream.advance();
  }

  if (!closed) {
    auto slice = stream.slice_from(start);

    // unit.diagnostics.emit({
    //     DiagnosticCode::UnterminatedString,
    //     slice
    // });

    return nullptr;
  }

  auto value = stream.slice_from(start);

  // consome '"'
  stream.advance();

  auto *descriptor = ctx.language.tokens.lookup_by_kind(TokenKind::STRING_LITERAL);

  return ctx.tokens.create_token<Token>(descriptor, value);
}