#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include "celestia/core/operators/UnaryOperation.hpp"

namespace celestia::ast {

struct UnaryExpressionNode : Expression {

  UnaryOperation op;
  Expression *operand;

  UnaryExpressionNode(UnaryOperation op, Expression *operand) : Expression(NodeKind::UnaryExpression), op(op), operand(operand) {}

  
};

} // namespace celestia::ast