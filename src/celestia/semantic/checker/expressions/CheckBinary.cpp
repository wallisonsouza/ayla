#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

namespace celestia::semantic {

void TypeChecker::binary_expression(ast::BinaryExpressionNode *node) {

  if (!node || !node->lhs || !node->rhs) return;

  auto &semantic = context.unit.semantic;

  check(node->lhs);
  check(node->rhs);

  TypeId lhs_type = semantic.type(node->lhs);
  TypeId rhs_type = semantic.type(node->rhs);

  if (!is_same_type(lhs_type, rhs_type)) {

    error(node, "binary expression requires operands of the same type");

    return;
  }

  semantic.set_type(node, lhs_type);

  debug::trace(debug::Category::TypeChecker, "binary expression type = {}", context.env().types.get(lhs_type).to_string());
}

} // namespace celestia::semantic