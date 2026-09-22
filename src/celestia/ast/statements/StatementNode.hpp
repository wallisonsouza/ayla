#pragma once
#include "celestia/ast/declarations/Declaration.hpp"

namespace celestia::ast {
struct Statement : BlockItem {
  explicit Statement(NodeKind k) : BlockItem(k) {}
};
} // namespace celestia::ast