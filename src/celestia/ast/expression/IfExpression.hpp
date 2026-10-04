#pragma once

#include "celestia/ast/expression/Expression.hpp"

namespace celestia::ast {

struct IfExpression : Expression {
  Expression *condition;
  Expression *then_block;
  Expression *else_block;

  IfExpression(Expression *cond, Expression *then_b, Expression *else_b = nullptr) : Expression(NodeKind::IfExpression), condition(cond), then_block(then_b), else_block(else_b) {}
};
} // namespace celestia::ast
