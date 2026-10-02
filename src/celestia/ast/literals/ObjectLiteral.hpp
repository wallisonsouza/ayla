#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include <vector>

namespace celestia::ast {

struct ObjectField : Node {
  Expression *key;
  Expression *value;

  ObjectField(Expression *k, Expression *v) : Node(NodeKind::ObjectField), key(k), value(v) {}
};

struct ObjectLiteral : Expression {
  std::vector<ObjectField *> fields;

  explicit ObjectLiteral(std::vector<ObjectField *> f) : Expression(NodeKind::ObjectLiteral), fields(std::move(f)) {}
};

} // namespace celestia::ast