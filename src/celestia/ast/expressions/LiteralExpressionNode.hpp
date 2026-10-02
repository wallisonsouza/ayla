#pragma once

#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/types/Type.hpp"
#include <string>
#include <vector>

namespace celestia::ast {

struct NumberLiteralNode : Expression {
  std::string value;

  explicit NumberLiteralNode(std::string value) : Expression(NodeKind::NumberLiteral), value(std::move(value)) {}
};

struct StringLiteralNode : Expression {
  std::string value;

  explicit StringLiteralNode(std::string v) : Expression(NodeKind::StringLiteral), value(std::move(v)) {}
};

struct BoolLiteralNode : Expression {
  bool value;

  explicit BoolLiteralNode(bool v) : Expression(NodeKind::BooleanLiteral), value(v) {}
};

struct NullLiteralNode : Expression {
  NullLiteralNode() : Expression(NodeKind::NullLiteral) {}
};

struct ArrayLiteralNode : Expression {
  std::vector<Expression *> elements;
  explicit ArrayLiteralNode(std::vector<Expression *> elems) : Expression(NodeKind::ArrayLiteral), elements(std::move(elems)) {}
};


} // namespace celestia::ast
