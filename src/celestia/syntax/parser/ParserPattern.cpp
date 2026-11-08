#include "celestia/ast/names/Qualified.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/syntax/parser/Parser.hpp"

namespace celestia::syntax {

Match<ast::PatternNode *> Parser::parse_pattern() {

  auto &tokens = context.tokens();
  std::cerr << "[Parser] parsing match arm\n";

  auto *current = tokens.current();

  if (current) {
    std::cerr << "[Pattern] current token kind = " << token_kind_name(current->kind())<< "\n";

    if (current->desc) { std::cerr << "[Pattern] current token name = " << current->desc->name << "\n"; }
  }

  if (!current) return Match<ast::PatternNode *>::no_match();

  if (current->kind() != TokenKind::IDENTIFIER) return Match<ast::PatternNode *>::no_match();

  auto name_result = parse_identifier();

  if (name_result.is_error()) return Match<ast::PatternNode *>::fail();

  if (name_result.is_no_match()) return Match<ast::PatternNode *>::no_match();

  auto *name = name_result.value();

  // Enum.Variant(...)
  if (tokens.current()->kind() == TokenKind::DOT) { return upcast<ast::PatternNode>(parse_enum_variant_pattern(name)); }

  // Named pattern:
  //
  // x
  // x: Int
  return upcast<ast::PatternNode>(parse_named_pattern(name));
}

Match<ast::NamedPattern *> Parser::parse_named_pattern(ast::Identifier *name) {

  auto &tokens = context.tokens();

  ast::Type *type = nullptr;

  if (tokens.match(TokenKind::COLON)) {

    auto type_result = parse_type();

    if (type_result.is_error()) return Match<ast::NamedPattern *>::fail();

    if (type_result.is_no_match()) {
      parser::diagnostics::report_expected_type(context);
      return Match<ast::NamedPattern *>::fail();
    }

    type = type_result.value();

  } else {

    auto type_result = speculate([&] { return parse_type(); });

    if (type_result.is_ok()) {
      parser::diagnostics::report_missing_pattern_colon(context);
      return Match<ast::NamedPattern *>::fail();
    }
  }

  auto *pattern = context.get_ast().alloc<ast::NamedPattern>(name, type);

  pattern->slice = name->slice;

  if (type) pattern->slice.extend_to(type->slice);

  return Match<ast::NamedPattern *>::ok(pattern);
}

Match<ast::EnumVariantPattern *> Parser::parse_enum_variant_pattern(ast::Identifier *first) {

  auto &tokens = context.tokens();

  std::vector<ast::Identifier *> parts;
  parts.push_back(first);

  while (tokens.match(TokenKind::DOT)) {

    auto result = parse_identifier();

    if (result.is_error()) return Match<ast::EnumVariantPattern *>::fail();

    if (result.is_no_match()) {
      parser::diagnostics::report_expected_identifier(context);
      return Match<ast::EnumVariantPattern *>::fail();
    }

    parts.push_back(result.value());
  }

  // Aqui você monta o QualifiedName a partir dos parts.
  auto *name = context.get_ast().alloc<ast::QualifiedName>(std::move(parts));

  std::vector<ast::PatternNode *> arguments;

  if (tokens.match(TokenKind::OPEN_PAREN)) {

    while (!tokens.is_end()) {

      if (tokens.match(TokenKind::CLOSE_PAREN)) break;

      auto pattern = parse_pattern();

      if (pattern.is_error() || pattern.is_no_match()) return Match<ast::EnumVariantPattern *>::fail();

      arguments.push_back(pattern.value());

      if (tokens.match(TokenKind::CLOSE_PAREN)) break;

      if (!tokens.match(TokenKind::COMMA)) return Match<ast::EnumVariantPattern *>::fail();
    }
  }

  auto *pattern = context.get_ast().alloc<ast::EnumVariantPattern>(name, std::move(arguments));

  return Match<ast::EnumVariantPattern *>::ok(pattern);
}

} // namespace celestia::syntax