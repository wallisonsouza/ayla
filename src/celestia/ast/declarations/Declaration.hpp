#pragma once
#include "celestia/ast/Node.hpp"

namespace celestia::ast {

struct BlockItem : Node {
  using Node::Node;
};

struct Declaration : BlockItem {
  explicit Declaration(NodeKind k) : BlockItem(k) {}
};

} // namespace celestia::ast