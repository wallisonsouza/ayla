#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/IfExpression.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::check_binary_expression(ast::BinaryExpression *node, ExpectedType expected) {
  assert(node);
  assert(node->lhs);
  assert(node->rhs);

  TypeId lhs_type = infer(node->lhs);
  if (!lhs_type.is_valid()) return TypeId::invalid();

  TypeId rhs_type = infer(node->rhs);
  if (!rhs_type.is_valid()) return TypeId::invalid();

  TypeId actual = type_system.binary_result_type(node->operation, lhs_type, rhs_type);

  if (!actual.is_valid()) {
    checker::diagnostics::report_invalid_binary_operation(context, node->lhs->slice, node->rhs->slice, node->operation, lhs_type, rhs_type);

    return TypeId::invalid();
  }

  if (expected.is_valid() && !type_system.is_assignable(expected.type, actual)) {
    checker::diagnostics::report_type_mismatch(context, expected.node ? expected.node->slice : node->slice, node->slice, expected.type, actual);

    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, actual);
  return actual;
}

TypeId TypeChecker::check_if_expression(ast::IfExpression *node, ExpectedType expected) {
  assert(node);
  assert(node->condition);
  assert(node->then_block);

  const TypeId bool_type = context.get_env().builtins.bool_type;
  const TypeId void_type = context.get_env().builtins.void_type;

  if (!check(node->condition, ExpectedType{.type = bool_type, .node = nullptr, .slice = SourceSlice::EMPTY}).is_valid()) { return TypeId::invalid(); }

  // Sem else, a expressão produz Void.
  if (!node->else_block) {
    TypeId then_type = check(node->then_block, ExpectedType{.type = void_type, .node = nullptr, .slice = SourceSlice::EMPTY});

    if (!then_type.is_valid()) return TypeId::invalid();

    if (expected.is_valid() && !type_system.is_assignable(expected.type, void_type)) {
      checker::diagnostics::report_type_mismatch(context, expected.node ? expected.node->slice : node->slice, node->slice, expected.type, void_type);

      return TypeId::invalid();
    }

    context.unit.semantic.set_type(node, void_type);
    return void_type;
  }

  // Com tipo esperado, propagamos a expectativa aos branches.
  if (expected.is_valid()) {
    TypeId then_type = check(node->then_block, expected);
    if (!then_type.is_valid()) return TypeId::invalid();

    TypeId else_type = check(node->else_block, expected);
    if (!else_type.is_valid()) return TypeId::invalid();

    context.unit.semantic.set_type(node, expected.type);
    return expected.type;
  }

  // Sem contexto, inferimos os dois branches.
  TypeId then_type = check(node->then_block, ExpectedType::none());
  if (!then_type.is_valid()) return TypeId::invalid();

  TypeId else_type = check(node->else_block, ExpectedType::none());
  if (!else_type.is_valid()) return TypeId::invalid();

  TypeId actual = type_system.common_type(then_type, else_type);

  if (!actual.is_valid()) {
    checker::diagnostics::report_type_mismatch(context, node->then_block->slice, node->else_block->slice, then_type, else_type);

    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, actual);
  return actual;
}

TypeId TypeChecker::check_block_expression(ast::BlockExpression *node, ExpectedType expected) {
  assert(node);

  for (auto *item : node->items) {
    if (!item) continue;

    if (!check(item, ExpectedType::none()).is_valid()) { return TypeId::invalid(); }
  }

  const TypeId void_type = context.get_env().builtins.void_type;

  if (!node->value) {
    if (expected.is_valid() && !type_system.is_assignable(expected.type, void_type)) {
      checker::diagnostics::report_type_mismatch(context, expected.node ? expected.node->slice : node->slice, node->slice, expected.type, void_type);

      return TypeId::invalid();
    }

    context.unit.semantic.set_type(node, void_type);
    return void_type;
  }

  TypeId actual = check(node->value, expected);

  if (!actual.is_valid()) return TypeId::invalid();

  context.unit.semantic.set_type(node, actual);
  return actual;
}

} // namespace celestia::semantic