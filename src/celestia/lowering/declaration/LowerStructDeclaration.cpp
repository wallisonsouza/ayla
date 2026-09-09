#include "celestia/lowering/Lowering.hpp"

#include <iostream>

namespace celestia::lowering {

void Lowering::lower_struct_declaration(const ast::StructDeclaration *node) {

  if (!node || !node->name) return;

  const auto &struct_name = node->name->str;

  std::cout << "[Lowering] struct '" << struct_name << "'\n";

  // --------------------------------------------------
  // Struct
  // --------------------------------------------------

  ir::StringId name = context.global().intern_string(struct_name);

  ir::StructId struct_id = context.ir.create_struct(name);

  if (!struct_id.is_valid()) {
    std::cout << "[Lowering] failed to create struct '" << struct_name << "'\n";
    return;
  }

  ir::TypeId struct_type = context.global().create_struct_type(context.unit.ir_module, struct_id);

  if (!struct_type.is_valid()) {
    std::cout << "[Lowering] failed to create IR type for struct '" << struct_name << "'\n";
    return;
  }

  std::cout << "[Lowering] struct '" << struct_name << "' -> StructId(" << struct_id.index() << "), TypeId(" << struct_type.index() << ")\n";

  // --------------------------------------------------
  // Semantic TypeId -> IR TypeId
  // --------------------------------------------------

  auto semantic_type_id = context.unit.semantic.type(node);

  if (semantic_type_id.is_valid()) {

    auto [it, inserted] = context.ir.get_program().type_map.emplace(semantic_type_id, struct_type);

    std::cout << "[Lowering] type map: semantic TypeId(" << semantic_type_id.index() << ") -> IR TypeId(" << it->second.index() << ")";

    if (!inserted) std::cout << " (already mapped)";

    std::cout << '\n';
  }

  // --------------------------------------------------
  // Fields
  // --------------------------------------------------

  for (const auto *field : node->fields) {

    if (!field || !field->name || !field->type) continue;

    const auto &field_name = field->name->str;

    const auto field_semantic_type_id = context.unit.semantic.type(field->type);

    if (!field_semantic_type_id.is_valid()) {

      std::cout << "[Lowering] field '" << struct_name << "." << field_name << "' has invalid semantic type\n";

      continue;
    }

    std::cout << "[Lowering] field '" << struct_name << "." << field_name << "' semantic TypeId(" << field_semantic_type_id.index() << ")\n";

    ir::TypeId field_type = lower_type(field_semantic_type_id);

    if (!field_type.is_valid()) {

      std::cout << "[Lowering] failed to lower field '" << struct_name << "." << field_name << "'\n";

      continue;
    }

    std::cout << "[Lowering] field '" << struct_name << "." << field_name << "' -> IR TypeId(" << field_type.index() << ")\n";

    ir::StringId field_id = context.global().intern_string(field_name);

    context.ir.add_struct_field(struct_id, field_id, field_type);
  }

  std::cout << "[Lowering] finished struct '" << struct_name << "'\n";
}

} // namespace celestia::lowering