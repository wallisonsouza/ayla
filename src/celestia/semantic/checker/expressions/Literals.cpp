#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

bool TypeChecker::check_number_literal(ast::NumberLiteralNode *node, TypeId expected) {

  assert(node);
  assert(expected.is_valid());

  TypeId actual = context.env().builtins.int_type;

  if (!is_assignable(expected, actual)) {
    checker::diagnostics::report_type_mismatch(context, node->slice, expected, actual);

    return false;
  }

  return true;
}

bool TypeChecker::check_string_literal(ast::StringLiteralNode *node, TypeId expected) {

  assert(node);
  assert(expected.is_valid());

  TypeId actual = context.env().builtins.string_type;

  if (!is_assignable(expected, actual)) {
    checker::diagnostics::report_type_mismatch(context, node->slice, expected, actual);

    return false;
  }

  return true;
}

bool TypeChecker::check_boolean_literal(ast::BoolLiteralNode *node, TypeId expected) {

  assert(node);
  assert(expected.is_valid());

  TypeId actual = context.env().builtins.bool_type;

  if (!is_assignable(expected, actual)) {
    checker::diagnostics::report_type_mismatch(context, node->slice, expected, actual);

    return false;
  }

  return true;
}

bool TypeChecker::check_array_literal(ast::ArrayLiteralNode *node, TypeId expected) {

  assert(node);
  assert(expected.is_valid());

  const auto &type = context.env().types.get(expected);

  if (type.kind != TypeKind::GenericInstance) {
    error(node, "expected an array type");
    return false;
  }

  const auto &array = static_cast<const GenericInstanceType &>(type);

  if (array.constructor != context.env().constructors.array_constructor) {
    error(node, "expected an array type");
    return false;
  }

  assert(array.arguments.size() == 1);

  TypeId element_type = array.arguments[0];

  for (auto *element : node->elements) {

    if (!element) continue;

    if (!check(element, element_type)) return false;
  }

  context.unit.semantic.set_type(node, expected);

  return true;
}


bool TypeChecker::check_struct_literal(ast::StructLiteralNode *node, TypeId expected) {

  assert(node);
  assert(expected.is_valid());

  const auto &type = context.env().types.get(expected);

  if (type.kind != TypeKind::Struct) {
    error(node, "expected a struct type");
    return false;
  }

  const auto &struct_type = static_cast<const StructType &>(type);

  for (auto *field : node->fields) {

    if (!field || !field->name) continue;

    const auto &name = field->name->str;

    if (!struct_type.has_member(name)) {
      error(field, "struct has no field '" + name + "'");
      return false;
    }

    if (!field->value) {
      error(field, "field has no value");
      return false;
    }

    TypeId field_type = struct_type.get_member(name);

    if (!check(field->value, field_type)) { return false; }
  }

  context.unit.semantic.set_type(node, expected);

  return true;

}

} // namespace celestia::semantic