#pragma once
#include "celestia/ast/expression/Expression.hpp"
#include "celestia/core/operators/BinaryOperation.hpp"

namespace celestia::ast {

struct BinaryExpression: Expression {

  Expression *lhs;
  BinaryOperation operation;
  Expression *rhs;

  BinaryExpression(Expression *l, BinaryOperation o, Expression *r) : Expression(NodeKind::BinaryExpression), lhs(l), operation(o), rhs(r) {}

  
};

} // namespace celestia::ast