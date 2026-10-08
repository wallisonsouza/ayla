#include "celestia/ast/NodeCast.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::infer(ast::Node *node) {
  assert(node);

  TypeId type = TypeId::invalid();

  switch (node->kind) {

  case ast::NodeKind::NumberLiteral: type = infer_number_literal(ast::as<ast::NumberLiteral>(node)); break;

  case ast::NodeKind::StringLiteral: type = infer_string_literal(ast::as<ast::StringLiteral>(node)); break;

  case ast::NodeKind::BooleanLiteral: type = infer_boolean_literal(ast::as<ast::BoolLiteral>(node)); break;

  case ast::NodeKind::ArrayLiteral: type = infer_array_literal(ast::as<ast::ArrayLiteral>(node)); break;

  case ast::NodeKind::StructLiteral: type = infer_struct_literal(ast::as<ast::StructLiteral>(node)); break;

  case ast::NodeKind::IfExpression: type = infer_if_expression(ast::as<ast::IfExpression>(node)); break;

  case ast::NodeKind::WhileExpression: type = infer_while_expression(ast::as<ast::WhileExpression>(node)); break;

  case ast::NodeKind::BinaryExpression: type = infer_binary_expression(ast::as<ast::BinaryExpression>(node)); break;

  case ast::NodeKind::IdentifierExpression: type = infer_identifier_expression(ast::as<ast::IdentifierExpressionNode>(node)); break;

  case ast::NodeKind::BlockExpression: type = infer_block_expression(ast::as<ast::BlockExpression>(node)); break;

  default: std::cerr << "unsupported expression inference: " << ast::node_kind_name(node->kind) << '\n'; break;
  }

  if (type.is_valid()) context.unit.semantic.set_type(node, type);

  return type;
}

TypeId TypeChecker::infer_identifier_expression(ast::IdentifierExpressionNode *node) {
  assert(node);

  SymbolId symbol = context.unit.semantic.symbol(node);

  if (!symbol.is_valid()) return TypeId::invalid();

  auto s = context.get_env().symbols.get(symbol);

  return s.type;
}

TypeId TypeChecker::infer_binary_expression(ast::BinaryExpression *node) {
  assert(node);
  assert(node->lhs);
  assert(node->rhs);

  TypeId lhs_type = infer(node->lhs);

  if (!lhs_type.is_valid()) return TypeId::invalid();

  TypeId rhs_type = infer(node->rhs);

  if (!rhs_type.is_valid()) return TypeId::invalid();

  return type_system.binary_result_type(node->operation, lhs_type, rhs_type);
}

TypeId TypeChecker::infer_block_expression(ast::BlockExpression *node) {

  assert(node);

  if (!node->value) return context.get_env().builtins.void_type;

  return infer(node->value);
}

TypeId TypeChecker::infer_if_expression(ast::IfExpression *node) {
  assert(node);
  assert(node->condition);
  assert(node->then_block);

  const TypeId bool_type = context.get_env().builtins.bool_type;
  const TypeId void_type = context.get_env().builtins.void_type;

  TypeId condition_type = infer(node->condition);

  if (!condition_type.is_valid()) return TypeId::invalid();

  if (!type_system.is_same_type(condition_type, bool_type)) return TypeId::invalid();

  TypeId then_type = infer(node->then_block);

  if (!then_type.is_valid()) return TypeId::invalid();

  if (!node->else_block) {
    if (!type_system.is_same_type(then_type, void_type)) return TypeId::invalid();

    return void_type;
  }

  TypeId else_type = infer(node->else_block);

  if (!else_type.is_valid()) return TypeId::invalid();

  return type_system.common_type(then_type, else_type);
}

TypeId TypeChecker::infer_while_expression(ast::WhileExpression *node) {
  assert(node);
  assert(node->condition);
  assert(node->body);

  const TypeId bool_type = context.get_env().builtins.bool_type;

  TypeId condition_type = infer(node->condition);

  if (!condition_type.is_valid()) return TypeId::invalid();

  if (!type_system.is_same_type(condition_type, bool_type)) return TypeId::invalid();

  TypeId body_type = infer(node->body);

  if (!body_type.is_valid()) return TypeId::invalid();

  return context.get_env().builtins.void_type;
}

TypeId TypeChecker::infer_number_literal(ast::NumberLiteral *node) {
  assert(node);
  return context.get_env().builtins.int_type;
}

TypeId TypeChecker::infer_string_literal(ast::StringLiteral *node) {
  assert(node);
  return context.get_env().builtins.string_type;
}

TypeId TypeChecker::infer_boolean_literal(ast::BoolLiteral *node) {
  assert(node);
  return context.get_env().builtins.bool_type;
}

TypeId TypeChecker::infer_array_literal(ast::ArrayLiteral *node) {
  assert(node);

  if (node->elements.empty()) return TypeId::invalid();

  TypeId element_type = infer(node->elements[0]);

  if (!element_type.is_valid()) return TypeId::invalid();

  for (size_t i = 1; i < node->elements.size(); ++i) {
    TypeId type = infer(node->elements[i]);

    if (!type.is_valid()) return TypeId::invalid();

    if (!type_system.is_assignable(element_type, type)) return TypeId::invalid();
  }

  return context.get_env().types.get_or_create_generic_instance(context.get_env().constructors.array_constructor, {element_type});
}

TypeId TypeChecker::infer_struct_literal(ast::StructLiteral *node) {
  assert(node);

  if (!node->type) return TypeId::invalid();

  return resolve_type(node->type);
}

} // namespace celestia::semantic