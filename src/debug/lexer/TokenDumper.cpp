#include "TokenDumper.hpp"
#include "celestia/core/source/SourceBuffer.hpp"
#include "celestia/core/token/TokenKind.hpp"
#include <iostream>
#include <sstream>

namespace celestia::debug {

std::string TokenDumper::sanitize_text(std::string text, size_t max) {
  for (char &c : text) {
    if (c == '\n')
      c = '~';
    else if (c == '\t')
      c = '>';
  }

  if (text.size() > max) text = text.substr(0, max) + "...";

  return text;
}

std::string TokenDumper::dump_token(const Token &token, const core::source::SourceBuffer &source) {

  const auto view = source.get_view(token.slice.get_span());

  std::ostringstream oss;

  oss << "Token {\n"
      << "  kind  : " << token_kind_name(token.kind()) << '\n'
      << "  group : " << token_group_name(token.group()) << '\n'
      << "  text  : \"" << sanitize_text(std::string(view)) << "\"\n"
      << "  range : " << token.slice.begin.line << ':' << token.slice.begin.column << " -> " << token.slice.end.line << ':' << token.slice.end.column << '\n'
      << "  offset: " << token.slice.begin.offset << " -> " << token.slice.end.offset << '\n'
      << '}';

  return oss.str();
}
std::string TokenDumper::dump(const core::token::TokenStream &tokens, const core::source::SourceBuffer &source) {

  std::ostringstream oss;

  for (size_t i = 0; i < tokens.size(); ++i) {

    const auto *token = tokens.at(i);

    if (!token) continue;

    oss << dump_token(*token, source) << "\n\n";
  }

  return oss.str();
}

} // namespace celestia::debug