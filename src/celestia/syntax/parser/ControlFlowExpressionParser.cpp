#include "celestia/ast/expressions/AssignmentExpression.hpp"
#include "celestia/ast/expressions/BinaryExpressionNode.hpp"
#include "celestia/ast/expressions/BlockExpression.hpp"
#include "celestia/ast/expressions/CallExpressionNode.hpp"
#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/expressions/IdentifierExpressionNode.hpp"
#include "celestia/ast/expressions/If.hpp"
#include "celestia/ast/expressions/IndexAcessExpressionNode.hpp"
#include "celestia/ast/expressions/MatchExpression.hpp"
#include "celestia/ast/expressions/MemberAccessExpressionNode.hpp"
#include "celestia/ast/expressions/UnaryExpressionNode.hpp"
#include "celestia/ast/expressions/While.hpp"
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

} // namespace celestia::syntax