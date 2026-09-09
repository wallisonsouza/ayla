#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

void TypeChecker::number_literal(ast::NumberLiteralNode *node) {

  if (!node) return;

  context.unit.semantic.set_type(node, context.env().builtins.int_type);
}

void TypeChecker::string_literal(ast::StringLiteralNode *node) {

  if (!node) return;

  context.unit.semantic.set_type(node, context.env().builtins.string_type);
}

void TypeChecker::boolean_literal(ast::BoolLiteralNode *node) {

  if (!node) return;

  context.unit.semantic.set_type(node, context.env().builtins.bool_type);
}

} // namespace celestia::semantic