#include "celestia/ast/NodeCast.hpp"
#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

ir::ValueId Lowering::lower_expression(const ast::Expression *node) {

  if (!node) return ir::ValueId::invalid();

  switch (node->kind) {

  case ast::NodeKind::NumberLiteral: return lower_number_literal(ast::as<ast::NumberLiteral>(node));

  case ast::NodeKind::StringLiteral: return lower_string_literal(ast::as<ast::StringLiteral>(node));

  case ast::NodeKind::BooleanLiteral: return lower_bool_literal(ast::as<ast::BoolLiteral>(node));

  // case ast::NodeKind::IdentifierExpression: return lower_identifier(ast::as<ast::IdentifierExpressionNode>(node));

  // case ast::NodeKind::BinaryExpression: return lower_binary_expression(ast::as<ast::BinaryExpression>(node));

  default: return ir::ValueId::invalid();
  }
}

} // namespace celestia::lowering