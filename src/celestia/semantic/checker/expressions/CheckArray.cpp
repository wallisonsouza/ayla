#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

namespace celestia::semantic {

void TypeChecker::array_literal(ast::ArrayLiteralNode *node) {

  if (!node) return;

  auto &semantic = context.unit.semantic;

  if (node->elements.empty()) {
    error(node, "cannot infer type of empty array");
    return;
  }

  check(node->elements[0]);

  TypeId element_type = semantic.type(node->elements[0]);

  if (!element_type.is_valid()) {
    error(node->elements[0], "array element has no valid type");
    return;
  }

  for (size_t i = 1; i < node->elements.size(); ++i) {

    check(node->elements[i]);

    TypeId type = semantic.type(node->elements[i]);

    if (!type.is_valid()) {
      error(node->elements[i], "array element has no valid type");
      return;
    }

    if (!is_assignable(element_type, type)) {
      error(node->elements[i], "array elements must have the same type");
      return;
    }
  }

  TypeId array_type = context.env().types.get_or_create_generic_instance(context.env().intrinsics.array, {element_type});

  if (!array_type.is_valid()) {
    error(node, "could not create array type");
    return;
  }

  semantic.set_type(node, array_type);

  debug::Trace::log(debug::Category::TypeChecker, "array type = {}", context.env().types.get(array_type).to_string());
}

} // namespace celestia::semantic