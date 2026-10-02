#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/statements/StatementNode.hpp"

namespace celestia::ast {

struct ReturnStatement : Statement {
  Expression *value = nullptr;

  ReturnStatement(Expression *v) : Statement(NodeKind::ReturnStatement), value(v) {}
};

} // namespace celestia::ast
