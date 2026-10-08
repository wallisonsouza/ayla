
#include "celestia/ast/expression/AssignmentExpression.hpp"
#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/CallExpression.hpp"
#include "celestia/ast/expression/IdentifierExpression.hpp"
#include "celestia/ast/expression/IndexAcessExpression.hpp"
#include "celestia/ast/expression/MemberAccessExpression.hpp"
#include "celestia/ast/expression/UnaryExpression.hpp"
#include "debug/ast/AstDumper.hpp"
#include <format>

namespace celestia::debug {

void AstDumper::dump_block_expression(const ast::BlockExpression *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.list("items", node->items);
  g.field("value", node->value);
}

void AstDumper::dump_if_expression(const ast::IfExpression *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("condition", node->condition);
  g.field("then", node->then_block);
  g.field("else", node->else_block);
}

void AstDumper::dump_call_expression(const ast::CallExpressionNode *node) {
  
  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Callee", node->callee);
  g.list("Arguments", node->arguments);
}

void AstDumper::dump_while_expression(const ast::WhileExpression *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Condition", node->condition);
  g.field("Body", node->body);
}

void AstDumper::dump_binary_expression(const ast::BinaryExpression*node) {

  auto g = context.object(std::format("BinaryExpression(\"{}\")", binary_operation_string(node->operation)));

  g.field("Left", node->lhs);
  g.field("Right", node->rhs);
}

void AstDumper::dump_unary_expression(const ast::UnaryExpressionNode *node) {
  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Operand", node->operand);
}

void AstDumper::dump_assignment_expression(const ast::AssignmentExpressionNode *node) {
  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Target", node->target);
  g.field("Value", node->value);
}

void AstDumper::dump_match_arm(const ast::MatchArm *node) {

  auto g = context.object(ast::node_kind_name(node->kind));
  g.field("pattern", node->pattern);
  g.field("body", node->body);
}

void AstDumper::dump_match_expression(const ast::MatchExpression *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("value", node->value);
  g.list("arms", node->arms);
}

void AstDumper::dump_index_acess_expression(const ast::IndexAccessExpressionNode *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Base", node->base);
  g.field("Index", node->index);
}

void AstDumper::dump_identifier_expression(const ast::IdentifierExpressionNode *node) {
  if (node->name) {
    auto g = context.object(std::format("IdentifierExpression(\"{}\")", node->name->str));

    (void)g;
  } else {
    auto g = context.object("IdentifierExpression");

    (void)g;
  }
}

void AstDumper::dump_member_acess_expression(const ast::MemberAccessExpressionNode *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Base", node->base);
  g.field("Field", node->member);
}
} // namespace celestia::debug