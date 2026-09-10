#include "celestia/ast/expressions/AssignmentExpression.hpp"
#include "celestia/ast/expressions/BinaryExpressionNode.hpp"
#include "celestia/ast/expressions/CallExpressionNode.hpp"
#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/expressions/IdentifierExpressionNode.hpp"
#include "celestia/ast/expressions/IndexAcessExpressionNode.hpp"
#include "celestia/ast/expressions/MemberAccessExpressionNode.hpp"
#include "celestia/ast/expressions/UnaryExpressionNode.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

namespace celestia::syntax {

celestia::ast::Expression *Parser::parse_expression() {
  auto *lhs = parse_unary_expression();

  if (!lhs) return nullptr;

  return parse_binary_expression(0, lhs);
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
    auto *token = context.tokens().current();

    if (!token) break;

    auto *info = context.operators().get(token->desc->kind);

    if (!info || info->kind != core::OperatorKind::Postfix) break;

    if (!std::holds_alternative<PostfixOperation>(info->op)) return nullptr;

    context.tokens().consume();

    auto op = std::get<PostfixOperation>(info->op);

    switch (op) {
    case PostfixOperation::Call: expr = parse_call(expr); break;

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

  if (!info || info->kind != core::OperatorKind::Prefix) { return parse_postfix_expression(); }

  if (!std::holds_alternative<UnaryOperation>(info->op)) return nullptr;

  context.tokens().consume();

  auto op = std::get<UnaryOperation>(info->op);

  auto *operand = parse_unary_expression();

  if (!operand) return nullptr;

  return context.get_ast().alloc<celestia::ast::UnaryExpressionNode>(op, operand);
}

celestia::ast::Expression *Parser::parse_identifier_expression() {

  auto name = parse_identifier_name();

  if (!name.is_ok()) return nullptr;

  if (context.tokens().check(TokenKind::OPEN_BRACE)) { return parse_struct_literal(name.value()); }

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

celestia::ast::Expression *Parser::parse_call(celestia::ast::Expression *callee) {
  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) return nullptr;

  std::vector<celestia::ast::Expression *> args;

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

  return context.get_ast().alloc<celestia::ast::CallExpressionNode>(callee, std::move(args));
}

} // namespace celestia::syntax