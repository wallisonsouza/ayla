#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

ir::ValueId Lowering::lower_number_literal(const ast::NumberLiteralNode *node) {

  if (!node) return ir::ValueId::invalid();

  auto semantic_type = context.unit.semantic.type(node);

  if (!semantic_type.is_valid()) return ir::ValueId::invalid();

  ir::TypeId type = lower_type(semantic_type);

  if (!type.is_valid()) return ir::ValueId::invalid();

  return context.ir.create_constant(type, node->value);
}

ir::ValueId Lowering::lower_string_literal(const ast::StringLiteralNode *node) {

  if (!node) return ir::ValueId::invalid();

  auto semantic_type = context.unit.semantic.type(node);

  if (!semantic_type.is_valid()) return ir::ValueId::invalid();

  ir::TypeId type = lower_type(semantic_type);

  if (!type.is_valid()) return ir::ValueId::invalid();

  return context.ir.create_constant(type, node->value);
}

ir::ValueId Lowering::lower_bool_literal(const ast::BoolLiteralNode *node) {

  if (!node) return ir::ValueId::invalid();

  auto semantic_type = context.unit.semantic.type(node);

  if (!semantic_type.is_valid()) return ir::ValueId::invalid();

  ir::TypeId type = lower_type(semantic_type);

  if (!type.is_valid()) return ir::ValueId::invalid();

  return context.ir.create_constant(type, node->value);
}

} // namespace celestia::lowering