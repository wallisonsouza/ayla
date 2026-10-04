#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include "celestia/ast/statements/StatementNode.hpp"

namespace celestia::ast {

struct ExpressionStatement : Statement {
  Expression *expression;

  explicit ExpressionStatement(Expression *expr) : Statement(NodeKind::ExpressionStatement), expression(expr) {}
};

} // namespace celestia::ast
