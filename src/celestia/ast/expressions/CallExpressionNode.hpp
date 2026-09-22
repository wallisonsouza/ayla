#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"
#include <vector>

namespace celestia::ast {

struct CallExpressionNode : Expression {

  Expression *callee;

  std::vector<TypeNode *> generics;
  std::vector<Expression *> arguments;

  CallExpressionNode(Expression *c, std::vector<Expression *> a) : Expression(NodeKind::Call), callee(c), arguments(std::move(a)) {}
};

} // namespace celestia::ast