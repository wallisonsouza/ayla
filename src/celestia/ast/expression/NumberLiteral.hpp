#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include <string>

namespace celestia::ast {

struct NumberLiteral : Expression {
  std::string value;

  explicit NumberLiteral(std::string value) : Expression(NodeKind::NumberLiteral), value(std::move(value)) {}
};

} // namespace celestia::ast
