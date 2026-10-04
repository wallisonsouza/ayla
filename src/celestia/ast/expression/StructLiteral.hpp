#pragma once

#include "celestia/ast/expression/Expression.hpp"
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {

struct StructField : Node {

  Identifier *name;
  Expression *value;

  StructField(Identifier *name, Expression *value) : Node(NodeKind::StructFieldInitializer), name(name), value(value) {}
};

struct StructLiteral : Expression {

  Type *type;

  std::vector<StructField *> fields;

  StructLiteral(Type *type, std::vector<StructField *> fields) : Expression(NodeKind::StructLiteral), type(type), fields(std::move(fields)) {}
};

} // namespace celestia::ast