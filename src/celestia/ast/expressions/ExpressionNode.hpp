#pragma once

#include "celestia/ast/Node.hpp"

namespace celestia::ast {

struct Expression : Node {
  explicit Expression(NodeKind k) : Node(k) {}
};

} // namespace celestia::ast