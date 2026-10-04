#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"

namespace celestia::semantic {

void TypeChecker::check_variable_declaration(ast::VariableDeclaration *node) {

  assert(node && node->pattern);

  TypeId declared_type = infer(node->pattern);
  
  TypeId initializer_type = TypeId::invalid();

  if (node->initializer) { initializer_type = infer(node->initializer); }

  // Nenhum tipo declarado e nenhum inicializador.
  if (!declared_type.is_valid() && !initializer_type.is_valid()) {
    checker::diagnostics::report_cannot_infer_type(context, node->slice);
    return;
  }

  // let value = 10
  if (!declared_type.is_valid()) { declared_type = initializer_type; }

  // let value: Int = "hello"
  if (initializer_type.is_valid() && !type_system.is_assignable(declared_type, initializer_type)) {

    checker::diagnostics::report_type_mismatch(context, node->initializer->slice, declared_type, initializer_type);

    return;
  }

  // Se quiser registrar o tipo do pattern.
  context.unit.semantic.set_type(node->pattern, declared_type);

  // E o tipo da declaração.
  context.unit.semantic.set_type(node, declared_type);
}
} // namespace celestia::semantic