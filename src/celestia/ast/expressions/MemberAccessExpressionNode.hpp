#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/names/Identifier.hpp"

namespace celestia::ast {

struct MemberAccessExpressionNode : Expression {
  Expression *base;
  Identifier *member;

  MemberAccessExpressionNode(Expression *b, Identifier *f) : Expression(NodeKind::MemberAccess), base(b), member(f) {}

  
};

} // namespace celestia::ast