#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/patterns/PatternNode.hpp"
#include <vector>

namespace celestia::ast {

struct MatchArm : Node {
  PatternNode *pattern;
  Expression *body;

  MatchArm(PatternNode *pattern, Expression *body) : Node(NodeKind::MatchArm), pattern(pattern), body(body) {}
};

struct MatchExpression : Expression {
  Expression *value;
  std::vector<MatchArm *> arms;

  MatchExpression(Expression *value, std::vector<MatchArm *> arms) : Expression(NodeKind::MatchExpression), value(value), arms(std::move(arms)) {}
};

} // namespace celestia::ast