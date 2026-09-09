

#pragma once

#include "celestia/ast/names/IdentifierNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"
#include <vector>

namespace celestia::ast {

// T
// T: Comparable
struct GenericParameterNode : Node {
  IdentifierNode *name;
  std::vector<TypeNode *> constraints;

  GenericParameterNode(IdentifierNode *name, std::vector<TypeNode *> constraints = {}) : Node(NodeKind::GenericParameter), name(name), constraints(std::move(constraints)) {}
};

// Iterator<T>
// Pair<T, U>
struct GenericIdentifierNode : NameNode {
  IdentifierNode *name;
  std::vector<GenericParameterNode *> parameters;

  GenericIdentifierNode(IdentifierNode *name, std::vector<GenericParameterNode *> parameters = {}) : NameNode(NodeKind::GenericIdentifier), name(name), parameters(std::move(parameters)) {}
};

} // namespace celestia::ast
