#include "celestia/ast/NodeCast.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::infer(ast::Node *node) {

  assert(node);

  TypeId type = TypeId::invalid();

  switch (node->kind) {

  case ast::NodeKind::NamedType: type = infer_named_type(ast::as<ast::NamedType>(node)); break;

  case ast::NodeKind::GenericType: type = infer_generic_type(ast::as<ast::GenericType>(node)); break;

  case ast::NodeKind::NumberLiteral: type = infer_number_literal(ast::as<ast::NumberLiteralNode>(node)); break;

  case ast::NodeKind::StringLiteral: type = infer_string_literal(ast::as<ast::StringLiteralNode>(node)); break;

  case ast::NodeKind::BooleanLiteral: type = infer_boolean_literal(ast::as<ast::BoolLiteralNode>(node)); break;

  case ast::NodeKind::ArrayLiteral: type = infer_array_literal(ast::as<ast::ArrayLiteralNode>(node)); break;

  case ast::NodeKind::StructLiteral: type = infer_struct_literal(ast::as<ast::StructLiteral>(node)); break;

  case ast::NodeKind::NamedPattern: type = infer_name_pattern(ast::as<ast::NamedPattern>(node)); break;

  case ast::NodeKind::IfExpression: type = infer_if_expression(ast::as<ast::IfExpression>(node)); break;

  case ast::NodeKind::WhileExpression: type = infer_while_expression(ast::as<ast::WhileExpression>(node)); break;
  
  case ast::NodeKind::BlockExpression: type = infer_block_expression(ast::as<ast::BlockExpression>(node)); break;

  default: std::cerr << "unsupported inference: " << ast::node_kind_name(node->kind) << '\n';
  }

  if (type.is_valid()) { context.unit.semantic.set_type(node, type); }

  return type;
}
} // namespace celestia::semantic