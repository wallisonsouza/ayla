#include "Lexer.hpp"

Token *Lexer::match_comment() {

  while (!stream.eof()) {
    char32_t c = stream.peek();
  
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

    break;
  }

  return nullptr;
  
}

