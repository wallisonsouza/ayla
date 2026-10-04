#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include "celestia/ast/statements/StatementNode.hpp"

namespace celestia::ast {

struct ReturnStatement : Statement {
  Expression *value = nullptr;

  ReturnStatement(Expression *v) : Statement(NodeKind::ReturnStatement), value(v) {}
};

} // namespace celestia::ast
