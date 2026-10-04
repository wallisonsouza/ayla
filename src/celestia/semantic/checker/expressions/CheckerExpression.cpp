#include "celestia/ast/expression/IfExpression.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

bool TypeChecker::check_if_expression(ast::IfExpression *node, TypeId expected) {

  assert(node);
  assert(node->condition);
  assert(node->then_block);

  TypeId bool_type = context.env().builtins.bool_type;

  if (!check(node->condition, bool_type)) return false;

  if (!check(node->then_block, expected)) return false;

  if (!node->else_block) {

    if (!expected.is_valid()) return true;

    return expected == context.env().builtins.void_type;
  }

  return check(node->else_block, expected);
}

bool TypeChecker::check_block_expression(ast::BlockExpression *node, TypeId expected) {

  assert(node);

  for (auto *item : node->items) {

    if (!check(item, TypeId::invalid())) return false;
  }

  if (!node->value) {

    auto void_type = context.env().builtins.void_type;

    if (!expected.is_valid()) return true;

    if (expected != void_type) {
      checker::diagnostics::report_type_mismatch(context, node->slice, expected, void_type);

      return false;
    }

    return true;
  }

  return check(node->value, expected);
}

} // namespace celestia::semantic
