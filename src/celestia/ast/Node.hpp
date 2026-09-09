#pragma once

#include "celestia/ast/NodeKind.hpp"
#include "celestia/core/token/Location.hpp"

namespace celestia::ast {

struct Node {

  const NodeKind kind;

  SourceSlice slice;

  virtual ~Node() = default;

protected:
  explicit Node(NodeKind k) : kind(k) {}
};

} // namespace celestia::ast