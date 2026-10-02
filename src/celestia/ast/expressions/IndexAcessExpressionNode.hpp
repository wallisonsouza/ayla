#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"

namespace celestia::ast {

struct IndexAccessExpressionNode : Expression {
  
  Expression *base;
  Expression *index;

  IndexAccessExpressionNode(Expression *b, Expression *i) : Expression(NodeKind::IndexAccess), base(b), index(i) {}
};

} // namespace celestia::ast