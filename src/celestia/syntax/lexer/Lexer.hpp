#pragma once

#include "celestia/core/source/TextStream.hpp"
#include "celestia/core/token/Token.hpp"
#include "celestia/syntax/lexer/LexerContext.hpp"
#include "celestia/utils/Unicode.hpp"

struct OperatorMatch {
  const celestia::TokenDescriptor *descriptor = nullptr;
  size_t length = 0;

  explicit operator bool() const { return descriptor != nullptr; }
};

struct Lexer {
private:
  LexerContext &ctx;
  core::source::TextStream stream;

  Token *match_token() {

    if (auto t = match_string()) return t;

    if (auto t = match_number()) return t;

    if (auto t = match_identifier()) return t;

    if (auto t = match_operator()) return t;

    return nullptr;
  }

  OperatorMatch find_operator();

public:
  Lexer(LexerContext &ctx) : ctx(ctx), stream(ctx.source.buffer) {};

  Token *match_identifier();
  Token *match_string();
  Token *match_number();
  Token *match_operator();
  Token mathch_comment();

  void skip_whitespace_and_comments(core::source::TextStream &stream) {
    while (!stream.eof()) {
      char32_t c = stream.peek();

      // Espaços em branco, mas NÃO newline
      if (utils::Unicode::is_white_space(c) && c != U'\n') {
        stream.advance();
        continue;
      }

      // Comentário de linha //
      if (c == U'/' && stream.peek_n(1) == U'/') {
        stream.advance_n(2);
        while (stream.peek() != U'\n' && !stream.eof()) stream.advance();
        continue;
      }

      // Comentário de bloco /* ... */
      if (c == U'/' && stream.peek_n(1) == U'*') {
        stream.advance_n(2);
        while (!stream.eof()) {
          if (stream.peek() == U'*' && stream.peek_n(1) == U'/') {
            stream.advance_n(2);
            break;
          }
          stream.advance();
        }
        continue;
      }

      break; // Não é espaço nem comentário
    }
  }

  void tokenize() {

    while (!stream.eof()) {
      skip_whitespace_and_comments(stream);

      if (stream.peek() == U'\n') {

        auto state = stream.get_state();

        auto desc = ctx.language.tokens.lookup_by_kind(TokenKind::NEW_LINE);
        ctx.tokens.create_token<Token>(desc, stream.slice_from(state));
      }

      auto start_state = stream.get_state();
      auto *token = match_token();

      if (!token) { stream.advance(); }
    }
  }
};
