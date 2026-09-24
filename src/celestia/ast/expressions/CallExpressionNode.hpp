#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {

struct CallExpressionNode : Expression {

  Expression *callee;

  std::vector<Type *> generic_arguments;
  std::vector<Expression *> arguments;

  CallExpressionNode(Expression *callee, std::vector<Type *> generic_arguments, std::vector<Expression *> arguments)
      : Expression(NodeKind::Call), callee(callee), generic_arguments(std::move(generic_arguments)), arguments(std::move(arguments)) {}
};
} // namespace celestia::ast