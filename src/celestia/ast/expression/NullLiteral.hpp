#pragma once

#include "celestia/ast/expression/Expression.hpp"

namespace celestia::ast {

struct NullLiteral : Expression {
  NullLiteral() : Expression(NodeKind::NullLiteral) {}
};

} // namespace celestia::ast
