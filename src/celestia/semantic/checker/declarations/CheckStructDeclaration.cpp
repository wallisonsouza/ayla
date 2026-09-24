#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

void TypeChecker::check_struct_declaration(ast::StructDeclaration *node) {

  if (!node) return;

  auto &semantic = context.unit.semantic;

  // --------------------------------------------------
  // Symbol
  // --------------------------------------------------

  SymbolId symbol_id = semantic.symbol(node);

  if (!symbol_id.is_valid()) {
    error(node, "struct has no resolved symbol");
    return;
  }

  // --------------------------------------------------
  // StructType
  // --------------------------------------------------

  TypeId type_id = context.env().types.get_or_create(symbol_id, TypeKind::Struct);

  if (!type_id.is_valid()) {
    error(node, "could not create struct type");
    return;
  }

  semantic.set_type(node, type_id);

  auto &symbol = context.env().symbols.get(symbol_id);

  symbol.type = type_id;

  auto &type = context.env().types.get(type_id);

  auto &struct_type = static_cast<StructType &>(type);

  // --------------------------------------------------
  // Compositions
  // --------------------------------------------------

  for (auto *composition : node->compositions) {

    if (!composition) continue;

    TypeId composed_type = infer(composition);

    if (!composed_type.is_valid()) {
      error(composition, "could not resolve struct composition type");
      continue;
    }

    semantic.set_type(composition, composed_type);

    auto &composed = context.env().types.get(composed_type);

    if (composed.kind != TypeKind::Struct) {
      error(composition, "struct composition must be a struct");
      continue;
    }

    auto &composed_struct = static_cast<const StructType &>(composed);

    for (const auto &[name, member_type] : composed_struct.members) {

      if (struct_type.has_member(name)) {
        error(composition, "duplicate field '" + name + "'");
        continue;
      }

      struct_type.add_member(name, member_type);
    }
  }

  // --------------------------------------------------
  // Fields
  // --------------------------------------------------

  for (auto *field : node->fields) {

    if (!field) continue;

    if (!field->name) {
      error(field, "struct field has no name");
      continue;
    }

    const auto &name = field->name->str;

    if (struct_type.has_member(name)) {
      error(field, "duplicate field '" + name + "'");
      continue;
    }

    if (!field->type) {
      error(field, "field '" + name + "' has no type");
      continue;
    }

    TypeId field_type = infer(field->type);

    if (!field_type.is_valid()) {
      error(field, "could not resolve field type");
      continue;
    }

    semantic.set_type(field->type, field_type);

    struct_type.add_member(name, field_type);
  }
}

} // namespace celestia::semantic