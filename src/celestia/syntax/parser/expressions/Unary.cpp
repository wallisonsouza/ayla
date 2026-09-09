#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/expressions/UnaryExpressionNode.hpp"
#include "celestia/syntax/parser/expressions/Expression.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

namespace celestia::syntax {
celestia::ast::Expression *ExpressionParser::parse_unary_expression() {
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
}