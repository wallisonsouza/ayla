#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include <vector>

namespace celestia::ast {

struct ArrayLiteral : Expression {
  std::vector<Expression *> elements;
  explicit ArrayLiteral(std::vector<Expression *> elems) : Expression(NodeKind::ArrayLiteral), elements(std::move(elems)) {}
};

} // namespace celestia::ast
