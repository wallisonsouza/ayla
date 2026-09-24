#pragma once

#include "celestia/ast/Node.hpp"

namespace celestia::ast {

struct Type : Node {
  explicit Type(NodeKind kind) : Node(kind) {}
};

} // namespace celestia::ast