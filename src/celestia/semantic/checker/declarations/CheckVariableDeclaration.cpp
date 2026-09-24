#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

void TypeChecker::check_variable_declaration(ast::VariableDeclaration *node) {

  assert(node && node->pattern);

  TypeId variable_type = TypeId::invalid();

  variable_type = infer(node->pattern);

  if (!variable_type.is_valid()) {

    if (!node->initializer) {
      checker::diagnostics::report_cannot_infer_type(context, node->slice);
      return;
    }

    variable_type = infer(node->initializer);

    if (!variable_type.is_valid()) { return; }
  }

  if (!check(node->pattern, variable_type)) return;

  if (node->initializer) {

    if (!check(node->initializer, variable_type)) return;
  }

  context.unit.semantic.set_type(node, variable_type);
}
} // namespace celestia::semantic