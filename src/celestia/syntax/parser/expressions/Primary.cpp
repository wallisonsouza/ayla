
#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/expressions/IdentifierExpressionNode.hpp"
#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"
#include "celestia/syntax/parser/expressions/Expression.hpp"
namespace celestia::syntax {
celestia::ast::Expression *ExpressionParser::parse_number_literal() {
  auto &tokens = context.tokens();

  auto *token = tokens.match(TokenKind::NUMBER_LITERAL);

  if (!token) return nullptr;

  auto text = context.source().buffer.get_text(token->slice.get_span());

  return context.get_ast().alloc<celestia::ast::NumberLiteralNode>(text);
}

celestia::ast::Expression *ExpressionParser::parse_string_literal() {
  auto *token = context.tokens().match(TokenKind::STRING_LITERAL);

  if (!token) return nullptr;

  auto text = context.source().buffer.get_text(token->slice.get_span());

  return context.get_ast().alloc<celestia::ast::StringLiteralNode>(text);
}

celestia::ast::Expression *ExpressionParser::parse_bool_literal() {
  auto *token = context.tokens().consume();

  if (!token) return nullptr;

  bool value = token->desc->kind == TokenKind::TRUE;

  return context.get_ast().alloc<celestia::ast::BoolLiteralNode>(value);
}

celestia::ast::Expression *ExpressionParser::parse_identifier_expression() {

  auto name = parser.parse_identifier_name();

  if (!name.is_ok()) return nullptr;

  if (context.tokens().check(TokenKind::OPEN_BRACE)) { return parse_struct_literal(name.value()); }

  return context.get_ast().alloc<celestia::ast::IdentifierExpressionNode>(name.value());
}

celestia::ast::Expression *ExpressionParser::parse_grouped_expression() {
  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) return nullptr;

  auto *expr = parse_expression();

  if (!expr) return nullptr;

  if (!tokens.match(TokenKind::CLOSE_PAREN)) return nullptr;

  return expr;
}

celestia::ast::Expression *ExpressionParser::parse_primary_expression() {

  auto *token = context.tokens().current();

  if (!token) return nullptr;

  switch (token->desc->kind) {

  case TokenKind::NUMBER_LITERAL: return parse_number_literal();

  case TokenKind::STRING_LITERAL: return parse_string_literal();

  case TokenKind::TRUE:
  case TokenKind::FALSE: return parse_bool_literal();

  case TokenKind::IDENTIFIER: return parse_identifier_expression();

  case TokenKind::OPEN_PAREN: return parse_grouped_expression();

  case TokenKind::OPEN_BRACKET: return parse_array_literal();

  default: return nullptr;
  }
}
celestia::ast::Expression *ExpressionParser::parse_struct_literal(celestia::ast::IdentifierNode *name) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_BRACE)) return nullptr;

  std::vector<celestia::ast::StructFieldInitializerNode *> fields;

  while (!tokens.check(TokenKind::CLOSE_BRACE)) {

    tokens.skip_trivia();

    auto *field_name = parser.parse_identifier().value();

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

celestia::ast::Expression *ExpressionParser::parse_array_literal() {

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