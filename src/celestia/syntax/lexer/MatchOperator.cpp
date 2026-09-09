#include "Lexer.hpp"
#include "celestia/core/token/TokenDescriptor.hpp"

OperatorMatch Lexer::find_operator() {
  const auto *node = ctx.language.tokens.trie().root();

  const celestia::TokenDescriptor *best = nullptr;
  size_t best_len = 0;

  for (size_t offset = 0;; ++offset) {
    char32_t c = stream.peek_n(offset);

    if (!c) break;

    node = node->child(c);

    if (!node) break;

    if (node->is_terminal) {
      best = node->value;
      best_len = offset + 1;
    }
  }

  return {
      .descriptor = best,
      .length = best_len,
  };
}

Token *Lexer::match_operator() {
  auto start = stream.get_state();

  auto match = find_operator();

  if (!match) return nullptr;

  stream.advance_n(match.length);

  auto slice = stream.slice_from(start);

  return ctx.tokens.create_token<Token>(match.descriptor, slice);
}