#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

void TypeChecker::check_struct_literal(ast::StructLiteralNode *node) {

  if (!node) return;

  auto &semantic = context.unit.semantic;

  SymbolId symbol_id = semantic.symbol(node);

  if (!symbol_id.is_valid()) {
    error(node, "struct literal has no resolved symbol");
    return;
  }

  auto &symbol = context.env().symbols.get(symbol_id);

  if (!symbol.type.is_valid()) {
    error(node, "struct literal has no resolved type");
    return;
  }

  TypeId struct_type_id = symbol.type;

  auto &type = context.env().types.get(struct_type_id);

  if (type.kind != TypeKind::Struct) {
    error(node, "struct literal requires a struct type");
    return;
  }

  auto &struct_type = static_cast<StructType &>(type);

  for (auto *field : node->fields) {

    if (!field || !field->name) continue;

    const auto &name = field->name->str;

    if (!struct_type.has_member(name)) {
      error(field, "struct has no field '" + name + "'");
      continue;
    }

    TypeId expected = struct_type.get_member(name);

    if (!field->value) {
      error(field, "field '" + name + "' has no value");
      continue;
    }

    check(field->value);

    TypeId actual = semantic.type(field->value);

    if (!actual.is_valid()) {
      error(field, "field '" + name + "' has no valid value type");
      continue;
    }

    if (!is_assignable(expected, actual)) {
      error(field, "field '" + name + "' has incompatible type");
      continue;
    }
  }

  semantic.set_type(node, struct_type_id);
}

} // namespace celestia::semantic