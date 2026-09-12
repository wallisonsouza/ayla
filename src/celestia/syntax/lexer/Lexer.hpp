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

    if (auto t = match_comment()) return t;

    if (auto t = match_string()) return t;

    if (auto t = match_number()) return t;

    if (auto t = match_identifier()) return t;

    if (auto t = match_newline()) return t;

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
  Token *match_comment();
  Token *match_newline();

  void tokenize() {

    while (!stream.eof()) {

      if (utils::Unicode::is_white_space(stream.peek()) && stream.peek() != U'\n') {
        stream.advance();
        continue;
      }

      auto *token = match_token();

      if (!token) { stream.advance(); }
    }
  }
};
