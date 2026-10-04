#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include <string>

namespace celestia::ast {

struct StringLiteral : Expression {
  std::string value;

  explicit StringLiteral(std::string v) : Expression(NodeKind::StringLiteral), value(std::move(v)) {}
};

} // namespace celestia::ast
