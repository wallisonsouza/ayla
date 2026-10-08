#include "celestia/ast/NodeCast.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

TypeId TypeChecker::build_type(ast::Node *node) {
  assert(node);

  switch (node->kind) {

  case ast::NodeKind::FunctionDeclaration: return build_function_type(ast::as<ast::FunctionDeclaration>(node));

  case ast::NodeKind::StructDeclaration:
    return build_struct_type(ast::as<ast::StructDeclaration>(node));

    //   case ast::NodeKind::EnumDeclaration: return build_enum_type(ast::as<ast::EnumDeclaration>(node));

    //   case ast::NodeKind::GenericDeclaration: return build_generic_type(ast::as<ast::GenericDeclaration>(node));

  default: error(node, "node cannot build a type"); return TypeId::invalid();
  }
}

TypeId TypeChecker::build_function_type(ast::FunctionDeclaration *node) {
  assert(node);

  auto &semantic = context.unit.semantic;

  SymbolId symbol_id = semantic.symbol(node);

  if (!symbol_id.is_valid()) {
    error(node, "function has invalid SymbolId");
    return TypeId::invalid();
  }

  TypeId function_type_id = context.get_env().types.get_or_create(symbol_id, TypeKind::Function);

  if (!function_type_id.is_valid()) {
    error(node, "could not create function type");
    return TypeId::invalid();
  }

  auto &type = context.get_env().types.get(function_type_id);

  auto &function_type = static_cast<FunctionType &>(type);

  // Evita duplicar parâmetros caso o tipo
  // seja construído novamente.
  function_type.params.clear();

  // Parâmetros
  for (auto *parameter : node->parameters) {

    if (!parameter) continue;

    TypeId parameter_type = resolve_pattern_type(parameter);

    if (!parameter_type.is_valid()) {
      error(parameter, "invalid parameter type");
      return TypeId::invalid();
    }

    function_type.params.push_back(parameter_type);

    semantic.set_type(parameter, parameter_type);
  }

  // Retorno
  if (node->return_type) {

    TypeId return_type = resolve_type(node->return_type);

    if (!return_type.is_valid()) {
      error(node->return_type, "invalid function return type");

      return TypeId::invalid();
    }

    function_type.return_type = return_type;
  }

  // O símbolo passa a apontar para o FunctionType.
  auto &symbol = context.get_env().symbols.get(symbol_id);
  symbol.type = function_type_id;

  return function_type_id;
}

TypeId TypeChecker::build_struct_type(ast::StructDeclaration *node) {
  assert(node);

  auto &semantic = context.unit.semantic;

  // --------------------------------------------------
  // Symbol
  // --------------------------------------------------

  SymbolId symbol_id = semantic.symbol(node);

  if (!symbol_id.is_valid()) {
    error(node, "struct has no resolved symbol");
    return TypeId::invalid();
  }

  // --------------------------------------------------
  // StructType
  // --------------------------------------------------

  TypeId type_id = context.get_env().types.get_or_create(symbol_id, TypeKind::Struct);

  if (!type_id.is_valid()) {
    error(node, "could not create struct type");
    return TypeId::invalid();
  }

  auto &symbol = context.get_env().symbols.get(symbol_id);
  symbol.type = type_id;

  auto &type = context.get_env().types.get(type_id);
  auto &struct_type = static_cast<StructType &>(type);

  // Evita acumular membros caso o tipo seja reconstruído.
  struct_type.members.clear();

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

    const auto &composed = context.get_env().types.get(composed_type);

    if (composed.kind != TypeKind::Struct) {
      error(composition, "struct composition must be a struct");
      continue;
    }

    const auto &composed_struct = static_cast<const StructType &>(composed);

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

    TypeId field_type = resolve_type(field->type);

    if (!field_type.is_valid()) {
      error(field, "could not resolve field type");
      continue;
    }

    semantic.set_type(field->type, field_type);

    struct_type.add_member(name, field_type);
  }

  return type_id;
}
} // namespace celestia::semantic