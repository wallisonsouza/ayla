#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/ast/declarations/ImportDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"
#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/ast/literals/StructLiteral.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"

namespace celestia::semantic {

TypeChecker::TypeChecker(TypeCheckerContext &context) : context(context), type_system(context.env()) {}

bool TypeChecker::check(ast::Node *node, TypeId expected) {

  if (!node) return false;

  switch (node->kind) {

  case ast::NodeKind::Root: check_root(static_cast<ast::RootNode *>(node)); return true;

  case ast::NodeKind::ModuleDeclaration: check_module_declaration(static_cast<ast::ModuleDeclaration *>(node)); return true;

  case ast::NodeKind::ImportDeclaration: check_import_declaration(static_cast<ast::ImportDeclaration *>(node)); return true;

  case ast::NodeKind::VariableDeclaration: check_variable_declaration(static_cast<ast::VariableDeclaration *>(node)); return true;

  case ast::NodeKind::FunctionDeclaration: check_function_declaration(static_cast<ast::FunctionDeclaration *>(node)); return true;

  case ast::NodeKind::StructDeclaration: check_struct_declaration(static_cast<ast::StructDeclaration *>(node)); return true;

  case ast::NodeKind::ModuleInitDeclaration: check_module_init_declaration(static_cast<ast::ModuleInitDeclaration *>(node)); return true;

  case ast::NodeKind::ReturnStatement: check_return_statement(static_cast<ast::ReturnStatement *>(node)); return true;

  case ast::NodeKind::NumberLiteral: return check_number_literal(static_cast<ast::NumberLiteralNode *>(node), expected);

  case ast::NodeKind::StringLiteral: return check_string_literal(static_cast<ast::StringLiteralNode *>(node), expected);

  case ast::NodeKind::BooleanLiteral: return check_boolean_literal(static_cast<ast::BoolLiteralNode *>(node), expected);

  case ast::NodeKind::ArrayLiteral: return check_array_literal(static_cast<ast::ArrayLiteralNode *>(node), expected);

  case ast::NodeKind::StructLiteral: return check_struct_literal(static_cast<ast::StructLiteral *>(node), expected);

  case ast::NodeKind::NamedPattern: return check_name_pattern(static_cast<ast::NamedPattern *>(node), expected);

  case ast::NodeKind::BlockExpression: return check_block_expression(static_cast<ast::BlockExpression *>(node), expected);

  default: error(node, "unsupported node in type checker: " + std::string(ast::node_kind_name(node->kind))); return false;
  }
}

} // namespace celestia::semantic