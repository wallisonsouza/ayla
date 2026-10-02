#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include <vector>

namespace celestia::ast {

struct ArrayLiteralNode : Expression {
  std::vector<Expression *> elements;
  explicit ArrayLiteralNode(std::vector<Expression *> elems) : Expression(NodeKind::ArrayLiteral), elements(std::move(elems)) {}
};

} // namespace celestia::ast
