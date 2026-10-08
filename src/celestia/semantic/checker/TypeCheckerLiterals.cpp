#include "celestia/ast/expression/ArrayLiteral.hpp"
#include "celestia/ast/expression/BoolLiteral.hpp"
#include "celestia/ast/expression/NumberLiteral.hpp"
#include "celestia/ast/expression/StringLiteral.hpp"
#include "celestia/ast/expression/StructLiteral.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::check_number_literal(ast::NumberLiteral *node, ExpectedType expected) {
  assert(node);

  TypeId actual = infer(node);

  if (expected.is_valid() && !type_system.is_assignable(expected.type, actual)) {

    checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, actual);

    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, actual);

  return actual;
}

TypeId TypeChecker::check_string_literal(ast::StringLiteral *node, ExpectedType expected) {
  assert(node);

  TypeId actual = infer(node);

  if (expected.is_valid() && !type_system.is_assignable(expected.type, actual)) {

    checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, actual);

    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, actual);

  return actual;
}

TypeId TypeChecker::check_boolean_literal(ast::BoolLiteral *node, ExpectedType expected) {
  assert(node);

  TypeId actual = infer(node);

  if (expected.is_valid() && !type_system.is_assignable(expected.type, actual)) {

    checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, actual);

    return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, actual);

  return actual;
}

TypeId TypeChecker::check_array_literal(ast::ArrayLiteral *node, ExpectedType expected) {
  assert(node);

  const TypeId array_constructor = context.get_env().constructors.array_constructor;

  if (!expected.is_valid()) {
    TypeId array_type = check_array_elements(node);

    if (!array_type.is_valid()) {
      if (node->elements.empty()) { checker::diagnostics::report_cannot_infer_type(context, node->slice); }

      return TypeId::invalid();
    }

    context.unit.semantic.set_type(node, array_type);
    return array_type;
  }

  const auto &expected_type = context.get_env().types.get(expected.type);

  if (expected_type.kind != TypeKind::GenericInstance) {

    TypeId actual = check_array_elements(node);

    if (actual.is_valid()) {

      checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, actual);
    } else if (node->elements.empty()) {
      // Not array
      checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, array_constructor);
    }

    return TypeId::invalid();
  }

  const auto &array_type = static_cast<const GenericInstanceType &>(expected_type);

  if (array_type.constructor != array_constructor) {
    TypeId actual = check_array_elements(node);

    if (!actual.is_valid()) return TypeId::invalid();

    checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, actual);

    return TypeId::invalid();
  }

  assert(array_type.arguments.size() == 1);

  const TypeId element_type = array_type.arguments[0];

  // O tipo esperado dos elementos vem do tipo do array.
  // O nó de origem continua sendo a anotação original,
  // quando ela existe.
  ExpectedType expected_element{.type = element_type, .node = expected.node, .slice = node->slice};

  for (auto *element : node->elements) {
    if (!element) continue;

    TypeId actual = check(element, expected_element);

    if (!actual.is_valid()) return TypeId::invalid();
  }

  context.unit.semantic.set_type(node, expected.type);
  return expected.type;
}

TypeId TypeChecker::check_array_elements(ast::ArrayLiteral *node) {
  if (node->elements.empty()) return TypeId::invalid();

  TypeId element_type = check(node->elements[0]);

  if (!element_type.is_valid()) return TypeId::invalid();

  for (size_t i = 1; i < node->elements.size(); ++i) {
    auto *element = node->elements[i];

    if (!element) continue;

    TypeId actual = check(element);

    if (!actual.is_valid()) return TypeId::invalid();

    if (!type_system.is_assignable(element_type, actual)) {
      checker::diagnostics::report_type_mismatch(context, element->slice, node->slice, element_type, actual);

      return TypeId::invalid();
    }
  }

  return context.get_env().types.get_or_create_generic_instance(context.get_env().constructors.array_constructor, {element_type});
}
TypeId TypeChecker::check_struct_literal(ast::StructLiteral *node, ExpectedType expected) {
  assert(node);

  if (!expected.is_valid()) { return infer(node); }

  const auto &type = context.get_env().types.get(expected.type);

  if (type.kind != TypeKind::Struct) {
    checker::diagnostics::report_type_mismatch(context, expected.slice, node->slice, expected.type, infer(node));

    return TypeId::invalid();
  }

  const auto &struct_type = static_cast<const StructType &>(type);

  for (auto *field : node->fields) {
    if (!field || !field->name) continue;

    const auto &name = field->name->str;

    if (!struct_type.has_member(name)) {
      error(field, "struct has no field '" + name + "'");
      return TypeId::invalid();
    }

    if (!field->value) {
      error(field, "field has no value");
      return TypeId::invalid();
    }

    TypeId field_type = struct_type.get_member(name);

    ExpectedType field_expected{.type = field_type, .node = expected.node};

    TypeId actual = check(field->value, field_expected);

    if (!actual.is_valid()) { return TypeId::invalid(); }
  }

  context.unit.semantic.set_type(node, expected.type);

  return expected.type;
}

} // namespace celestia::semantic