#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::infer(ast::Node *node) {

  assert(node);

  TypeId type = TypeId::invalid();

  switch (node->kind) {

  case ast::NodeKind::NamedType: type = infer_named_type(static_cast<ast::NamedType *>(node)); break;

  case ast::NodeKind::GenericType: type = infer_generic_type(static_cast<ast::GenericType *>(node)); break;

  case ast::NodeKind::NumberLiteral: type = infer_number_literal(static_cast<ast::NumberLiteralNode *>(node)); break;

  case ast::NodeKind::StringLiteral: type = infer_string_literal(static_cast<ast::StringLiteralNode *>(node)); break;

  case ast::NodeKind::BooleanLiteral: type = infer_boolean_literal(static_cast<ast::BoolLiteralNode *>(node)); break;

  case ast::NodeKind::ArrayLiteral: type = infer_array_literal(static_cast<ast::ArrayLiteralNode *>(node)); break;

  case ast::NodeKind::StructLiteral: type = infer_struct_literal(static_cast<ast::StructLiteralNode *>(node)); break;

  case ast::NodeKind::NamedPattern: type = infer_name_pattern(static_cast<ast::NamedPattern *>(node)); break;

  default: std::cerr << "unsupported inference: " << ast::node_kind_name(node->kind) << '\n'; assert(false);
  }

  if (type.is_valid()) { context.unit.semantic.set_type(node, type); }

  return type;
}

TypeId TypeChecker::infer_name_pattern(ast::NamedPattern *pattern) {

  assert(pattern);

  if (!pattern->type_annotation) return TypeId::invalid();

  return infer(pattern->type_annotation);
}
TypeId TypeChecker::infer_number_literal(ast::NumberLiteralNode *node) { return context.env().builtins.int_type; }

TypeId TypeChecker::infer_string_literal(ast::StringLiteralNode *node) { return context.env().builtins.string_type; }

TypeId TypeChecker::infer_boolean_literal(ast::BoolLiteralNode *node) { return context.env().builtins.bool_type; }

TypeId TypeChecker::infer_array_literal(ast::ArrayLiteralNode *node) {

  if (node->elements.empty()) {
    checker::diagnostics::report_cannot_infer_type(context, node->slice);
    return TypeId::invalid();
  }

  TypeId element_type = infer(node->elements[0]);

  if (!element_type.is_valid()) {
    error(node->elements[0], "array element has no valid type");
    return TypeId::invalid();
  }

  for (size_t i = 1; i < node->elements.size(); ++i) {

    TypeId type = infer(node->elements[i]);

    if (!type.is_valid()) {
      error(node->elements[i], "array element has no valid type");
      return TypeId::invalid();
    }

    if (!is_assignable(element_type, type)) {
      error(node->elements[i], "array elements must have the same type");

      return TypeId::invalid();
    }
  }

  TypeId array_type = context.env().types.get_or_create_generic_instance(context.env().constructors.array_constructor, {element_type});

  if (!array_type.is_valid()) {
    error(node, "could not create array type");
    return TypeId::invalid();
  }

  debug::Trace::log(debug::Category::TypeChecker, "array literal -> {}", context.env().types.get(array_type).to_string());

  return array_type;
}

TypeId TypeChecker::infer_struct_literal(ast::StructLiteralNode *node) {

  assert(node);

  if (!node->type) {
    error(node, "struct literal has no type");
    return TypeId::invalid();
  }

  TypeId struct_type_id = infer(node->type);

  if (!struct_type_id.is_valid()) {
    error(node, "could not resolve struct type");
    return TypeId::invalid();
  }

  const auto &type = context.env().types.get(struct_type_id);

  if (type.kind != TypeKind::Struct) {
    error(node, "struct literal requires a struct type");
    return TypeId::invalid();
  }

  const auto &struct_type = static_cast<const StructType &>(type);

  for (auto *field : node->fields) {

    if (!field) continue;

    if (!field->name) {
      error(field, "struct field has no name");
      return TypeId::invalid();
    }

    const auto &name = field->name->str;

    if (!struct_type.has_member(name)) {
      error(field, "unknown field '" + name + "'");
      return TypeId::invalid();
    }

    TypeId expected_type = struct_type.get_member(name);

    if (!expected_type.is_valid()) {
      error(field, "field has no valid type");
      return TypeId::invalid();
    }

    if (!check(field->value, expected_type)) { return TypeId::invalid(); }
  }

  return struct_type_id;
}

TypeId TypeChecker::infer_generic_type(ast::GenericType *node) {

  if (!node || !node->name) return TypeId::invalid();

  // Resolver already resolved the generic name.
  SymbolId symbol = context.unit.semantic.symbol(node);

  if (!symbol.is_valid()) {
    error(node, "invalid generic constructor symbol");
    return TypeId::invalid();
  }

  TypeId constructor = context.env().symbols.get(symbol).type;

  if (!constructor.is_valid()) {
    error(node, "generic constructor has no type");
    return TypeId::invalid();
  }

  const auto &constructor_type = context.env().types.get(constructor);

  if (constructor_type.kind != TypeKind::Generic) {
    error(node, "symbol is not a generic type");
    return TypeId::invalid();
  }

  const auto &generic = static_cast<const GenericDeclarationType &>(constructor_type);

  if (node->arguments.size() != generic.arity) {
    error(node, "wrong number of generic arguments");
    return TypeId::invalid();
  }

  std::vector<TypeId> arguments;

  for (auto *argument : node->arguments) {

    if (!argument) {
      error(node, "invalid generic argument");
      return TypeId::invalid();
    }

    TypeId argument_type = infer(argument);

    if (!argument_type.is_valid()) {
      error(argument, "invalid generic argument type");
      return TypeId::invalid();
    }

    arguments.push_back(argument_type);
  }

  TypeId type = context.env().types.get_or_create_generic_instance(constructor, arguments);

  if (!type.is_valid()) {
    error(node, "could not create generic instance");
    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, type);

  return type;
}

TypeId TypeChecker::infer_named_type(ast::NamedType *node) {

  if (!node) return TypeId::invalid();

  SymbolId symbol_id = context.unit.semantic.symbol(node);

  if (!symbol_id.is_valid()) {

    error(node, "named type symbol not found");
    return TypeId::invalid();
  }

  auto &symbol = context.env().symbols.get(symbol_id);

  if (!symbol.type.is_valid()) {

    error(node, "symbol has no valid type");
    return TypeId::invalid();
  }

  return symbol.type;
}

} // namespace celestia::semantic