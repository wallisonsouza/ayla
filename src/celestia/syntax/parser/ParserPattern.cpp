#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/syntax/parser/Parser.hpp"

namespace celestia::syntax {

// funciona mas tenho que revisitar
ParseResult<ast::PatternNode *> Parser::parse_pattern() {
  auto &tokens = context.tokens();

  auto *current = tokens.current();

  if (!current) return ParseResult<ast::PatternNode *>::no_match();

  switch (current->kind()) {
  case TokenKind::IDENTIFIER:
    return upcast<ast::PatternNode>(parse_named_pattern());

    // case TokenKind::OPEN_BRACKET:
    //   return parse_array_pattern();

    // case TokenKind::OPEN_BRACE:
    //   return parse_object_pattern();

  default: return ParseResult<ast::PatternNode *>::no_match();
  }
}

// ok terminei essa parte
ParseResult<ast::NamedPattern *> Parser::parse_named_pattern() {
  auto &tokens = context.tokens();

  auto name_result = parse_identifier();

  if (name_result.is_no_match()) return ParseResult<ast::NamedPattern *>::no_match();

  if (name_result.is_error()) return ParseResult<ast::NamedPattern *>::fail();

  auto *name = name_result.value();

  ast::TypeNode *type = nullptr;

  if (tokens.match(TokenKind::COLON)) {
    auto type_result = parse_type();

    if (type_result.is_error()) return ParseResult<ast::NamedPattern *>::fail();

    if (type_result.is_no_match()) {
      parser::diagnostics::report_expected_type(context);
      return ParseResult<ast::NamedPattern *>::fail();
    }

    type = type_result.value();

  } else {
    auto type_result = speculate([&] { return parse_type(); });

    if (type_result.is_ok()) {
      parser::diagnostics::report_missing_pattern_colon(context);
      return ParseResult<ast::NamedPattern *>::fail();
    }
  }

  auto *pattern = context.get_ast().alloc<ast::NamedPattern>(name, type);

  return ParseResult<ast::NamedPattern *>::ok(pattern);
}

} // namespace celestia::syntax