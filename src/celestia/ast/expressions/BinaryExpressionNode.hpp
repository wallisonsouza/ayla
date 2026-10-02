#pragma once
#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/core/operators/BinaryOperation.hpp"

namespace celestia::ast {

struct BinaryExpressionNode : Expression {

  Expression *lhs;
  BinaryOperation operation;
  Expression *rhs;

  BinaryExpressionNode(Expression *l, BinaryOperation o, Expression *r) : Expression(NodeKind::BinaryExpression), lhs(l), operation(o), rhs(r) {}

  
};

} // namespace celestia::ast