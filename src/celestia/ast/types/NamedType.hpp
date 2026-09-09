#pragma once

#include "celestia/ast/names/NameNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"

namespace celestia::ast {

struct NamedType : TypeNode {

  NameNode *name;

  bool is_primitive;

  NamedType(NameNode *name, bool primitive = false) : TypeNode(NodeKind::NamedType), name(name), is_primitive(primitive) {}
};

} // namespace celestia::ast