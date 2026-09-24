#pragma once
#include "celestia/ast/Node.hpp"

namespace celestia::ast {

struct NameNode : Node {
  explicit NameNode(NodeKind k) : Node(k) {}

  virtual std::string get_str() const = 0;
};

} // namespace celestia::ast