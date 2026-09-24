
#include "celestia/ast/names/Qualified.hpp"
#include "celestia/syntax/parser/Parser.hpp"

namespace celestia::syntax {

ParseResult<celestia::ast::NameNode *> Parser::parse_name() {

  auto &tokens = context.tokens();

  auto first = parse_identifier();

  if (first.is_no_match()) return ParseResult<celestia::ast::NameNode *>::no_match();

  if (first.is_error()) return ParseResult<celestia::ast::NameNode *>::fail();

  if (!tokens.check(TokenKind::DOT)) return ParseResult<celestia::ast::NameNode *>::ok(first.value());

  std::vector<celestia::ast::Identifier *> parts;
  parts.push_back(first.value());

  while (tokens.match(TokenKind::DOT)) {

    auto next = parse_identifier();

    if (next.is_no_match()) {
      parser::diagnostics::report_expected_identifier(context);
      return ParseResult<celestia::ast::NameNode *>::fail();
    }

    if (next.is_error()) return ParseResult<celestia::ast::NameNode *>::fail();

    parts.push_back(next.value());
  }

  auto *qualified = context.get_ast().alloc<celestia::ast::QualifiedName>(std::move(parts));

  qualified->slice.begin = qualified->parts.front()->slice.begin;
  qualified->slice.end = qualified->parts.back()->slice.end;

  return ParseResult<celestia::ast::NameNode *>::ok(qualified);
}
ParseResult<celestia::ast::Identifier *> Parser::parse_identifier() {
  auto &tokens = context.tokens();

  auto *token = tokens.match(TokenKind::IDENTIFIER);

  if (!token) { return ParseResult<celestia::ast::Identifier *>::no_match(); }

  auto text = context.source().buffer.get_text(token->slice.get_span());

  auto *node = context.get_ast().alloc<celestia::ast::Identifier>(text);

  node->slice = token->slice;

  return ParseResult<celestia::ast::Identifier *>::ok(node);
}

ParseResult<ast::Identifier *> Parser::parse_identifier_name() {

  auto result = parse_name();

  if (result.is_error()) { return ParseResult<ast::Identifier *>::fail(); }

  if (result.is_no_match()) { return ParseResult<ast::Identifier *>::no_match(); }

  auto *name = result.value();

  if (name->kind != ast::NodeKind::Identifier) { return ParseResult<ast::Identifier *>::no_match(); }

  return ParseResult<ast::Identifier *>::ok(static_cast<ast::Identifier *>(name));
}

} // namespace celestia::syntax
