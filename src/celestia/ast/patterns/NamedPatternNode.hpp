#pragma once

#include "PatternNode.hpp"
#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/expressions/ExpressionNode.hpp"
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {

struct EnumVariantPattern : PatternNode {
  QualifiedName *name;
  std::vector<PatternNode *> arguments;

  EnumVariantPattern(QualifiedName *name, std::vector<PatternNode *> arguments) : PatternNode(NodeKind::EnumVariantPattern), name(name), arguments(std::move(arguments)) {}
};

// struct LiteralPattern : PatternNode {
//   Expression *value;

//   LiteralPattern(Expression *value) : PatternNode(NodeKind::LiteralPattern), value(value) {}
// };

// struct TuplePattern : PatternNode {
//   std::vector<PatternNode *> elements;
// };

struct WildcardPattern : PatternNode {
  WildcardPattern() : PatternNode(NodeKind::WildcardPattern) {}
};

struct NamedPattern : PatternNode {
  Identifier *name;
  Type *type_annotation;

  NamedPattern(Identifier *n, Type *type = nullptr) : PatternNode(NodeKind::NamedPattern), name(n), type_annotation(type) {}
};

} // namespace celestia::ast