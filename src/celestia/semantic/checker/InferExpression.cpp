#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/IfExpression.hpp"
#include "celestia/ast/expression/WhileExpression.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::infer_binary_expression(ast::BinaryExpressionNode *node) {

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

  if (!node->value) return context.env().builtins.void_type;

  return infer(node->value);
}

TypeId TypeChecker::infer_if_expression(ast::IfExpression *node) {

  assert(node);
  assert(node->condition);
  assert(node->then_block);

  TypeId then_type = infer(node->then_block);

  if (!then_type.is_valid()) return TypeId::invalid();

  if (!node->else_block) return context.env().builtins.void_type;

  TypeId else_type = infer(node->else_block);

  if (!else_type.is_valid()) return TypeId::invalid();

  return type_system.common_type(then_type, else_type);
}

TypeId TypeChecker::infer_while_expression(ast::WhileExpression *node) {

  assert(node);
  assert(node->condition);
  assert(node->body);

  return context.env().builtins.void_type;
}

} // namespace celestia::semantic