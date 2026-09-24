
#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/names/Identifier.hpp"

namespace celestia::ast {

struct IdentifierExpressionNode : Expression {

  Identifier *name;
  explicit IdentifierExpressionNode(Identifier *n) : Expression(NodeKind::IdentifierExpression), name(n){}

  
};
} // namespace celestia::ast*