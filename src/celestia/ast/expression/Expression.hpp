#pragma once

#include "celestia/ast/declaration/Declaration.hpp"

namespace celestia::ast {

struct Expression : BlockItem {
  explicit Expression(NodeKind k) : BlockItem(k) {}
};

} // namespace celestia::ast