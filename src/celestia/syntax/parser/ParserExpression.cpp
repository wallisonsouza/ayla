#include "celestia/ast/expression/AssignmentExpression.hpp"
#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/CallExpression.hpp"
#include "celestia/ast/expression/Expression.hpp"
#include "celestia/ast/expression/IdentifierExpression.hpp"
#include "celestia/ast/expression/IndexAcessExpression.hpp"
#include "celestia/ast/expression/MemberAccessExpression.hpp"
#include "celestia/ast/expression/UnaryExpression.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"
#include "celestia/ast/expression/BlockExpression.hpp"
#include "celestia/ast/expression/Expression.hpp"
#include "celestia/ast/expression/IfExpression.hpp"
#include "celestia/ast/expression/MatchExpression.hpp"
#include "celestia/ast/expression/WhileExpression.hpp"
#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

namespace celestia::syntax {

ast::Expression *Parser::parse_match_expression() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::MATCH)) return nullptr;

  auto *value = parse_expression();

  if (!value) return nullptr;

  if (!tokens.match(TokenKind::OPEN_BRACE)) return nullptr;

  tokens.skip_trivia();

  std::vector<ast::MatchArm *> arms;

  while (!tokens.is_end()) {

    if (tokens.match(TokenKind::CLOSE_BRACE)) break;

    auto pattern_result = parse_pattern();

    if (pattern_result.is_error() || pattern_result.is_no_match()) return nullptr;

    auto *pattern = pattern_result.value();

    if (!tokens.match(TokenKind::ARROW)) return nullptr;

    auto *body = parse_expression();

    if (!body) return nullptr;

    arms.push_back(context.get_ast().alloc<ast::MatchArm>(pattern, body));

    tokens.skip_trivia();
  }

  return context.get_ast().alloc<ast::MatchExpression>(value, std::move(arms));
}

// while
ast::WhileExpression *Parser::parse_while_expression() {
  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::WHILE_KEYWORD)) return nullptr;

  auto *condition = parse_expression();

  if (!condition) {
    // context.//report_error(
    //     DiagnosticCode::ConditionMissing,
    //     "expected condition after while"
    // );

    return nullptr;
  }

  if (condition->kind == ast::NodeKind::Assignment) {
    // context.//report_error(
    //     DiagnosticCode::ConditionAssignment,
    //     "assignment is not allowed in while condition"
    // );

    return nullptr;
  }

  auto *block = parse_block_expression();

  if (!block) {
    // context.//report_error(
    //     DiagnosticCode::BlockError,
    //     "error in while block"
    // );

    return nullptr;
  }

  return context.get_ast().alloc<celestia::ast::WhileExpression>(condition, block);
}

// if
celestia::ast::Expression *Parser::parse_if_expression() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::IF_KEYWORD)) return nullptr;

  tokens.skip_trivia();

  auto *condition = parse_expression();

  if (!condition) {
    parser::diagnostics::report_expected_expression(context);
    return nullptr;
  }

  if (condition->kind == celestia::ast::NodeKind::Assignment) {
    // context.report_error(...)
    return nullptr;
  }

  tokens.skip_trivia();

  auto *then_block = parse_block_expression();

  if (!then_block) {
    // erro
    return nullptr;
  }

  tokens.skip_trivia();

  celestia::ast::Expression *else_block = nullptr;

  if (tokens.match(TokenKind::ELSE_KEYWORD)) {

    tokens.skip_trivia();

    if (tokens.check(TokenKind::IF_KEYWORD)) {
      else_block = parse_if_expression();
    } else {
      else_block = parse_block_expression();
    }

    if (!else_block) {
      // erro: esperado if ou bloco após else
      return nullptr;
    }
  }

  return context.get_ast().alloc<celestia::ast::IfExpression>(condition, then_block, else_block);
}

ast::BlockExpression *Parser::parse_block_expression() {

  auto &tokens = context.tokens();

  auto *open = tokens.current();

  if (!tokens.match(TokenKind::OPEN_BRACE)) return nullptr;

  auto start = open->slice;

  tokens.skip_trivia();

  auto result = parse_block_items();

  if (!result.is_ok()) return nullptr;

  auto parsed = std::move(result.value());

  ast::Expression *value = nullptr;
  std::vector<ast::BlockItem *> items;

  // `_` means that the block has no value.
  if (tokens.match(TokenKind::UNDERSCORE)) {

    tokens.skip_trivia();

    auto *close = tokens.current();

    if (!tokens.match(TokenKind::CLOSE_BRACE)) return nullptr;

    auto end = close->slice;

    for (auto *node : parsed) {

      if (dynamic_cast<ast::Expression *>(node)) {

        auto *expr = static_cast<ast::Expression *>(node);

        items.push_back(context.get_ast().alloc<ast::ExpressionStatement>(expr));

        continue;
      }

      items.push_back(static_cast<ast::BlockItem *>(node));
    }

    auto *block = context.get_ast().alloc<ast::BlockExpression>(std::move(items), value);

    block->slice = start;
    block->slice.extend_to(end);

    return block;
  }

  auto *close = tokens.current();

  if (!tokens.match(TokenKind::CLOSE_BRACE)) return nullptr;

  auto end = close->slice;

  if (!parsed.empty()) {

    auto *last = parsed.back();

    if (dynamic_cast<ast::Expression *>(last)) {
      value = static_cast<ast::Expression *>(last);
      parsed.pop_back();
    }
  }

  for (auto *node : parsed) {

    if (dynamic_cast<ast::Expression *>(node)) {

      auto *expr = static_cast<ast::Expression *>(node);

      items.push_back(context.get_ast().alloc<ast::ExpressionStatement>(expr));

      continue;
    }

    items.push_back(static_cast<ast::BlockItem *>(node));
  }

  auto *block = context.get_ast().alloc<ast::BlockExpression>(std::move(items), value);

  block->slice = start;
  block->slice.extend_to(end);

  return block;
}

ParseResult<std::vector<ast::BlockItem *>> Parser::parse_block_items() {

  auto &tokens = context.tokens();

  std::vector<ast::BlockItem *> items;

  while (!tokens.is_end()) {

    tokens.skip_trivia();

    if (tokens.check(TokenKind::CLOSE_BRACE)) break;
    if (tokens.check(TokenKind::UNDERSCORE)) break;

    ast::BlockItem *item = nullptr;

    auto decl = parse_declaration();

    if (decl.is_ok()) {
      item = decl.value();

    } else if (auto *stmt = parse_statement()) {
      item = stmt;

    } else if (auto *expr = parse_expression()) {
      item = expr;

    } else {
      return ParseResult<std::vector<ast::BlockItem *>>::fail();
    }

    items.push_back(item);

    tokens.skip_trivia();

    if (tokens.check(TokenKind::CLOSE_BRACE)) break;

    if (tokens.check(TokenKind::UNDERSCORE)) break;

    if (!tokens.match(TokenKind::NEW_LINE)) return ParseResult<std::vector<ast::BlockItem *>>::fail();

    tokens.skip_trivia();
  }

  return ParseResult<std::vector<ast::BlockItem *>>::ok(std::move(items));
}

celestia::ast::Expression *Parser::parse_expression() {
  auto *lhs = parse_unary_expression();

  if (!lhs) return nullptr;

  return parse_binary_expression(0, lhs);
}

celestia::ast::Expression *Parser::parse_grouped_expression() {
  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) return nullptr;

  auto *expr = parse_expression();

  if (!expr) return nullptr;

  if (!tokens.match(TokenKind::CLOSE_PAREN)) return nullptr;

  return expr;
}

ast::Expression *Parser::parse_primary_expression() {

  auto *token = context.tokens().current();

  if (!token) return nullptr;

  switch (token->desc->kind) {

  case TokenKind::NUMBER_LITERAL: return parse_number_literal();

  case TokenKind::STRING_LITERAL: return parse_string_literal();

  case TokenKind::TRUE:
  case TokenKind::FALSE: return parse_literal_expression();

  // Names
  case TokenKind::IDENTIFIER: return parse_identifier_expression();

  // Grouping / collection
  case TokenKind::OPEN_PAREN: return parse_grouped_expression();

  case TokenKind::OPEN_BRACKET: return parse_array_literal();

  // Control flow / blocks
  case TokenKind::OPEN_BRACE: return parse_block_expression();

  case TokenKind::IF_KEYWORD: return parse_if_expression();

  case TokenKind::WHILE_KEYWORD: return parse_while_expression();

  case TokenKind::MATCH: return parse_match_expression();

  default: return nullptr;
  }
}

celestia::ast::Expression *Parser::parse_assignment(celestia::ast::Expression *target) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::ASSIGN)) return nullptr;

  auto *value = parse_expression();

  if (!value) return nullptr;

  return context.get_ast().alloc<celestia::ast::AssignmentExpressionNode>(target, value);
}

celestia::ast::Expression *Parser::parse_binary_expression(int min_bp, celestia::ast::Expression *left) {

  while (true) {

    auto *token = context.tokens().current();

    if (!token) break;

    auto *info = context.operators().get(token->desc->kind);

    if (!info || info->kind != core::OperatorKind::Infix) break;

    if (info->lbp < min_bp) break;

    if (!std::holds_alternative<BinaryOperation>(info->op)) return nullptr;

    context.tokens().consume();

    auto *right = parse_unary_expression();

    if (!right) return nullptr;

    right = parse_binary_expression(info->rbp, right);

    auto op = std::get<BinaryOperation>(info->op);

    left = context.get_ast().alloc<celestia::ast::BinaryExpressionNode>(left, op, right);
  }

  return left;
}

celestia::ast::Expression *Parser::parse_postfix_expression() {

  auto *expr = parse_primary_expression();

  if (!expr) return nullptr;

  while (true) {

    auto &tokens = context.tokens();
    auto *token = tokens.current();

    if (!token) break;

    /*
     * foo<T, U>(...)
     */
    if (token->desc->kind == TokenKind::LESS) {

      auto generic_arguments = parse_type_arguments();

      if (!generic_arguments) return nullptr;

      if (tokens.current()->desc->kind != TokenKind::OPEN_PAREN) return nullptr;

      expr = parse_call(expr, std::move(*generic_arguments));

      if (!expr) return nullptr;

      continue;
    }

    auto *info = context.operators().get(token->desc->kind);

    if (!info || info->kind != core::OperatorKind::Postfix) break;

    if (!std::holds_alternative<PostfixOperation>(info->op)) return nullptr;

    tokens.consume();

    auto op = std::get<PostfixOperation>(info->op);

    switch (op) {

    case PostfixOperation::Call: expr = parse_call(expr, {}); break;

    case PostfixOperation::IndexAccess: expr = parse_index_access(expr); break;

    case PostfixOperation::MemberAccess: expr = parse_member_access(expr); break;

    default: return nullptr;
    }

    if (!expr) return nullptr;
  }

  return expr;
}

celestia::ast::Expression *Parser::parse_member_access(celestia::ast::Expression *base) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::DOT)) return nullptr;

  auto *member = parse_identifier().value();

  if (!member) return nullptr;

  return context.get_ast().alloc<celestia::ast::MemberAccessExpressionNode>(base, member);
}

celestia::ast::Expression *Parser::parse_unary_expression() {

  auto *token = context.tokens().current();

  if (!token) return parse_postfix_expression();

  auto *info = context.operators().get(token->desc->kind);

  if (!info || info->kind != core::OperatorKind::Prefix) return parse_postfix_expression();

  if (!std::holds_alternative<UnaryOperation>(info->op)) return nullptr;

  context.tokens().consume();

  auto op = std::get<UnaryOperation>(info->op);

  auto *operand = parse_unary_expression();

  if (!operand) return nullptr;

  return context.get_ast().alloc<celestia::ast::UnaryExpressionNode>(op, operand);
}

ast::Expression *Parser::parse_identifier_expression() {

  auto name = parse_identifier_name();

  if (!name.is_ok()) return nullptr;

  return context.get_ast().alloc<celestia::ast::IdentifierExpressionNode>(name.value());
}

celestia::ast::Expression *Parser::parse_index_access(celestia::ast::Expression *base) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_BRACKET)) return nullptr;

  auto *index = parse_expression();

  if (!index) return nullptr;

  if (!tokens.match(TokenKind::CLOSE_BRACKET)) return nullptr;

  return context.get_ast().alloc<celestia::ast::IndexAccessExpressionNode>(base, index);
}

std::optional<std::vector<ast::Type *>> Parser::parse_type_arguments() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::LESS)) return std::nullopt;

  std::vector<ast::Type *> arguments;

  while (!tokens.is_end()) {

    auto *type = parse_type().value();

    if (!type) return std::nullopt;

    arguments.push_back(type);

    if (tokens.match(TokenKind::GREATER)) break;

    if (!tokens.match(TokenKind::COMMA)) return std::nullopt;
  }

  return arguments;
}

ast::CallExpressionNode *Parser::parse_call(celestia::ast::Expression *callee, std::vector<ast::Type *> generic_arguments) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) return nullptr;

  std::vector<ast::Expression *> args;

  while (!tokens.is_end()) {

    if (tokens.match(TokenKind::CLOSE_PAREN)) break;

    auto *expr = parse_expression();

    if (!expr) return nullptr;

    args.push_back(expr);

    if (!tokens.match(TokenKind::COMMA)) {

      if (!tokens.match(TokenKind::CLOSE_PAREN)) return nullptr;

      break;
    }
  }

  return context.get_ast().alloc<ast::CallExpressionNode>(callee, std::move(generic_arguments), std::move(args));
}

} // namespace celestia::syntax