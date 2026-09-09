#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

namespace celestia::semantic {

void TypeChecker::check_variable_declaration(ast::VariableDeclaration *node) {

  if (!node) return;

  debug::trace(debug::Category::TypeChecker, "checking variable declaration");

  if (!node->pattern) {
    error(node, "variable has no pattern");
    return;
  }

  // --------------------------------------------------
  // Initializer
  // --------------------------------------------------

  TypeId initializer_type = TypeId::invalid();

  if (node->initializer) {

    debug::trace(debug::Category::TypeChecker, "checking variable initializer");

    check(node->initializer);

    initializer_type = context.unit.semantic.type(node->initializer);

    if (!initializer_type.is_valid()) {
      error(node, "initializer has no valid type");
      return;
    }

    debug::trace(debug::Category::TypeChecker, "initializer type = {}", context.env().types.get(initializer_type).to_string());
  }

  // --------------------------------------------------
  // Pattern
  // --------------------------------------------------

  TypeId variable_type = check_pattern(node->pattern, initializer_type);

  if (!variable_type.is_valid()) return;

  // --------------------------------------------------
  // Resultado
  // --------------------------------------------------

  context.unit.semantic.set_type(node, variable_type);

  debug::trace(debug::Category::TypeChecker, "variable type = {}", context.env().types.get(variable_type).to_string());
}

} // namespace celestia::semantic