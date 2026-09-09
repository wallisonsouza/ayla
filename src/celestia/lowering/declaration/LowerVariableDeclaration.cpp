#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

void Lowering::lower_variable_declaration(const ast::VariableDeclaration *node) {

  if (!node || !node->pattern) return;

  if (!node->initializer) return;

  ir::ValueId value = lower_expression(node->initializer);

  if (!value.is_valid()) return;

  lower_pattern(node->pattern, value);
}

} // namespace celestia::lowering