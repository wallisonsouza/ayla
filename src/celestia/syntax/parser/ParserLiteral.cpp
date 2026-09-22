#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/syntax/parser/Parser.hpp"

namespace celestia::syntax {

celestia::ast::Expression *Parser::parse_number_literal() {
  auto &tokens = context.tokens();

  auto *token = tokens.match(TokenKind::NUMBER_LITERAL);

  if (!token) return nullptr;

  auto text = context.source().buffer.get_text(token->slice.get_span());

  return context.get_ast().alloc<celestia::ast::NumberLiteralNode>(text);
}

celestia::ast::StringLiteralNode *Parser::parse_string_literal() {
  auto *token = context.tokens().match(TokenKind::STRING_LITERAL);

  if (!token) return nullptr;

  auto text = context.source().buffer.get_text(token->slice.get_span());

  return context.get_ast().alloc<celestia::ast::StringLiteralNode>(text);
}

celestia::ast::Expression *Parser::parse_bool_literal() {
  auto *token = context.tokens().consume();

  if (!token) return nullptr;

  bool value = token->desc->kind == TokenKind::TRUE;

  return context.get_ast().alloc<celestia::ast::BoolLiteralNode>(value);
}

ast::Expression *Parser::parse_literal_expression() {
  auto *token = context.tokens().current();

  if (!token) return nullptr;

  switch (token->desc->kind) {
  case TokenKind::NUMBER_LITERAL: return parse_number_literal();

  case TokenKind::STRING_LITERAL: return parse_string_literal();

  case TokenKind::TRUE:
  case TokenKind::FALSE: return parse_bool_literal();

  default: return nullptr;
  }
}


celestia::ast::Expression *Parser::parse_struct_literal(celestia::ast::IdentifierNode *name) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_BRACE)) return nullptr;

  std::vector<celestia::ast::StructFieldInitializerNode *> fields;

  while (!tokens.check(TokenKind::CLOSE_BRACE)) {

    tokens.skip_trivia();

    auto *field_name = parse_identifier().value();

    if (!field_name) return nullptr;

    if (!tokens.match(TokenKind::COLON)) return nullptr;

    auto *value = parse_expression();

    if (!value) return nullptr;

    fields.push_back(context.get_ast().alloc<celestia::ast::StructFieldInitializerNode>(field_name, value));

    tokens.skip_trivia();

    // vírgula opcional
    if (tokens.match(TokenKind::COMMA)) {
      tokens.skip_trivia();

      // trailing comma
      if (tokens.check(TokenKind::CLOSE_BRACE)) break;

      continue;
    }

    // sem vírgula, o próximo campo deve começar
    if (tokens.check(TokenKind::IDENTIFIER)) continue;

    break;
  }

  if (!tokens.match(TokenKind::CLOSE_BRACE)) return nullptr;

  auto *type = context.get_ast().alloc<celestia::ast::NamedType>(name);

  return context.get_ast().alloc<celestia::ast::StructLiteralNode>(type, std::move(fields));
}

celestia::ast::Expression *Parser::parse_array_literal() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_BRACKET)) return nullptr;

  std::vector<celestia::ast::Expression *> elements;

  tokens.skip_trivia();

  // []
  if (tokens.match(TokenKind::CLOSE_BRACKET)) { return context.get_ast().alloc<celestia::ast::ArrayLiteralNode>(std::move(elements)); }

  while (!tokens.check(TokenKind::CLOSE_BRACKET)) {

    tokens.skip_trivia();

    auto *element = parse_expression();

    if (!element) return nullptr;

    elements.push_back(element);

    tokens.skip_trivia();

    if (tokens.match(TokenKind::COMMA)) {

      tokens.skip_trivia();

      // trailing comma:
      // [1, 2, 3,]
      if (tokens.check(TokenKind::CLOSE_BRACKET)) break;

      continue;
    }

    if (!tokens.check(TokenKind::CLOSE_BRACKET)) return nullptr;
  }

  if (!tokens.match(TokenKind::CLOSE_BRACKET)) return nullptr;

  return context.get_ast().alloc<celestia::ast::ArrayLiteralNode>(std::move(elements));
}

} // namespace celestia::syntax
