#pragma once

#include "celestia/ast/expression/Expression.hpp"

namespace celestia::ast {

struct WhileExpression : Expression {
  
  Expression *condition;
  Expression *body;

  WhileExpression(Expression *cond, Expression *block) : Expression(NodeKind::WhileExpression), condition(cond), body(block) {}
};

} // namespace celestia::ast
