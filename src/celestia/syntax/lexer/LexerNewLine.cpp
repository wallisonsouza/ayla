#include "celestia/syntax/lexer/Lexer.hpp"

Token *Lexer::match_newline() {

  if (stream.peek() != U'\n') return nullptr;

  auto state = stream.get_state();

  stream.advance();

  auto desc = ctx.language.tokens.lookup_by_kind(TokenKind::NEW_LINE);

  return ctx.tokens.create_token<Token>(desc, stream.slice_from(state));
}