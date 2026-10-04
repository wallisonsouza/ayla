#pragma once
#include "celestia/ast/declaration/Declaration.hpp"

namespace celestia::ast {

struct Statement : BlockItem {
  explicit Statement(NodeKind k) : BlockItem(k) {}
};

} // namespace celestia::ast