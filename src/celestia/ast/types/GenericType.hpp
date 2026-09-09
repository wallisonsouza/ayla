#pragma once

#include "celestia/ast/names/NameNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"
#include <vector>

namespace celestia::ast {

struct GenericTypeNode : TypeNode {

  NameNode *name;
  std::vector<TypeNode *> arguments;

  GenericTypeNode(NameNode *name, std::vector<TypeNode *> arguments) : TypeNode(NodeKind::GenericType), name(name), arguments(std::move(arguments)) {}
};
} // namespace celestia::ast