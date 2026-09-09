#include "celestia/ast/NodeCast.hpp"
#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

void Lowering::lower_declaration(const ast::Declaration *node) {

  if (!node) return;

  switch (node->kind) {

  case ast::NodeKind::VariableDeclaration: lower_variable_declaration(ast::as<ast::VariableDeclaration>(node)); return;
  case ast::NodeKind::StructDeclaration: lower_struct_declaration(ast::as<ast::StructDeclaration>(node)); return;
  case ast::NodeKind::FunctionDeclaration: lower_function_declaration(ast::as<ast::FunctionDeclaration>(node)); return;
    case ast::NodeKind::ModuleDeclaration: lower_module_declaration(ast::as<ast::ModuleDeclaration>(node)); return;
  default: return;
  }
}

} // namespace celestia::lowering