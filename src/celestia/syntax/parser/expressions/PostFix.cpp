
#include "celestia/ast/expressions/CallExpressionNode.hpp"
#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/expressions/IndexAcessExpressionNode.hpp"
#include "celestia/ast/expressions/MemberAccessExpressionNode.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"
#include "celestia/syntax/parser/expressions/Expression.hpp"

namespace celestia::syntax {
celestia::ast::Expression *ExpressionParser::parse_postfix_expression() {
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

celestia::ast::Expression *ExpressionParser::parse_member_access(celestia::ast::Expression *base) {
  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::DOT)) return nullptr;

  auto *member = parser.parse_identifier().value();

  if (!member) return nullptr;

  return context.get_ast().alloc<celestia::ast::MemberAccessExpressionNode>(base, member);
}

celestia::ast::Expression *ExpressionParser::parse_index_access(celestia::ast::Expression *base) {
  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_BRACKET)) return nullptr;

  auto *index = parse_expression();

  if (!index) return nullptr;

  if (!tokens.match(TokenKind::CLOSE_BRACKET)) return nullptr;

  return context.get_ast().alloc<celestia::ast::IndexAccessExpressionNode>(base, index);
}

celestia::ast::Expression *ExpressionParser::parse_call(celestia::ast::Expression *callee) {
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