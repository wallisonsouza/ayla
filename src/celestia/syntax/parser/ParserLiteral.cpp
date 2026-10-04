#include "celestia/ast/expression/ArrayLiteral.hpp"
#include "celestia/ast/expression/BoolLiteral.hpp"
#include "celestia/ast/expression/NumberLiteral.hpp"
#include "celestia/ast/expression/StringLiteral.hpp"
#include "celestia/ast/expression/StructLiteral.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/syntax/parser/Parser.hpp"

namespace celestia::syntax {

ast::Expression *Parser::parse_number_literal() {

  auto *token = context.tokens().match(TokenKind::NUMBER_LITERAL);

  if (!token) return nullptr;

  auto text = context.source().buffer.get_text(token->slice.get_span());

  auto *node = context.get_ast().alloc<ast::NumberLiteral>(text);

  node->slice = token->slice;

  return node;
}
ast::Expression *Parser::parse_string_literal() {

  auto *token = context.tokens().match(TokenKind::STRING_LITERAL);

  if (!token) return nullptr;

  auto text = context.source().buffer.get_text(token->slice.get_span());

  auto *node = context.get_ast().alloc<ast::StringLiteral>(text);

  node->slice = token->slice;

  return node;
}
ast::Expression *Parser::parse_bool_literal() {

  auto *token = context.tokens().consume();

  if (!token) return nullptr;

  bool value = token->desc->kind == TokenKind::TRUE;

  auto *node = context.get_ast().alloc<ast::BoolLiteral>(value);

  node->slice = token->slice;

  return node;
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

ast::Expression *Parser::parse_struct_literal(ast::Identifier *name) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_BRACE)) return nullptr;

  std::vector<ast::StructField *> fields;

  while (!tokens.check(TokenKind::CLOSE_BRACE)) {

    tokens.skip_trivia();

    auto *field_name = parse_identifier().value();

    if (!field_name) return nullptr;

    if (!tokens.match(TokenKind::COLON)) return nullptr;

    auto *value = parse_expression();

    if (!value) return nullptr;

    fields.push_back(context.get_ast().alloc<ast::StructField>(field_name, value));

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

  return context.get_ast().alloc<celestia::ast::StructLiteral>(type, std::move(fields));
}

celestia::ast::Expression *Parser::parse_array_literal() {

  auto &tokens = context.tokens();

  auto *open = tokens.match(TokenKind::OPEN_BRACKET);

  if (!open) return nullptr;

  std::vector<celestia::ast::Expression *> elements;

  tokens.skip_trivia();

  if (tokens.match(TokenKind::CLOSE_BRACKET)) {

    auto *node = context.get_ast().alloc<celestia::ast::ArrayLiteral>(std::move(elements));

    node->slice.begin = open->slice.begin;
    node->slice.end = tokens.previous()->slice.end;

    return node;
  }

  while (!tokens.check(TokenKind::CLOSE_BRACKET)) {

    tokens.skip_trivia();

    auto *element = parse_expression();

    if (!element) return nullptr;

    elements.push_back(element);

    tokens.skip_trivia();

    if (tokens.match(TokenKind::COMMA)) {

      tokens.skip_trivia();

      if (tokens.check(TokenKind::CLOSE_BRACKET)) break;

      continue;
    }

    if (!tokens.check(TokenKind::CLOSE_BRACKET)) return nullptr;
  }

  auto *close = tokens.match(TokenKind::CLOSE_BRACKET);

  if (!close) return nullptr;

  auto *node = context.get_ast().alloc<celestia::ast::ArrayLiteral>(std::move(elements));

  node->slice.begin = open->slice.begin;
  node->slice.end = close->slice.end;

  return node;
}
} // namespace celestia::syntax
