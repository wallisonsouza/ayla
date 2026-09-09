#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

void Lowering::lower_pattern(const ast::PatternNode *node, ir::ValueId value) {

  if (!node) return;

  switch (node->kind) {

  case ast::NodeKind::NamedPattern: {
    // auto *pattern = static_cast<const ast::NamedPattern *>(node);

    // if (!pattern->symbol_id.is_valid()) return;

    // if (!value.is_valid()) return;

    // values[pattern->symbol_id] = value;
    // return;
  }

  default: return;
  }
}

} // namespace celestia::lowering