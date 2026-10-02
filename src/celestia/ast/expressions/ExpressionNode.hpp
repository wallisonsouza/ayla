#pragma once

#include "celestia/ast/declarations/Declaration.hpp"

namespace celestia::ast {

struct Expression : BlockItem {
  explicit Expression(NodeKind k) : BlockItem(k) {}
};

} // namespace celestia::ast