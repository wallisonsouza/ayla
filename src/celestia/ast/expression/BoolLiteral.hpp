#pragma once

#include "celestia/ast/expression/Expression.hpp"

namespace celestia::ast {

struct BoolLiteral : Expression {
  bool value;

  explicit BoolLiteral(bool v) : Expression(NodeKind::BooleanLiteral), value(v) {}
};

} // namespace celestia::ast
