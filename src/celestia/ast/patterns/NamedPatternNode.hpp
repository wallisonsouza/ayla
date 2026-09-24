#pragma once

#include "celestia/ast/types/Type.hpp"
#include "celestia/ast/names/Identifier.hpp"
#include "PatternNode.hpp"

namespace celestia::ast {

struct NamedPattern : PatternNode {
  Identifier *name;
  Type *type_annotation;

  NamedPattern(Identifier *n, Type *type = nullptr) : PatternNode(NodeKind::NamedPattern), name(n), type_annotation(type) {}
};

} // namespace celestia::ast