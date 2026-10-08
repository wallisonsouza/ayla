#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/ast/declaration/FunctionDeclaration.hpp"
#include "celestia/ast/declaration/ImportDeclaration.hpp"
#include "celestia/ast/declaration/StructDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"
#include "celestia/ast/expression/ArrayLiteral.hpp"
#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/BoolLiteral.hpp"
#include "celestia/ast/expression/IfExpression.hpp"
#include "celestia/ast/expression/NumberLiteral.hpp"
#include "celestia/ast/expression/StringLiteral.hpp"
#include "celestia/ast/expression/StructLiteral.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"

namespace celestia::semantic {

TypeChecker::TypeChecker(SemanticContext &context) : context(context), type_system(context.get_env()) {}

TypeId TypeChecker::check(ast::Node *node, ExpectedType expected) {

  if (!node) return TypeId::invalid();

  const TypeId VOID_TYPE = context.get_env().builtins.void_type;

  switch (node->kind) {

  case ast::NodeKind::Root: check_root(static_cast<ast::RootNode *>(node)); return VOID_TYPE;

  case ast::NodeKind::ModuleDeclaration: check_module_declaration(static_cast<ast::ModuleDeclaration *>(node)); return VOID_TYPE;

  case ast::NodeKind::ImportDeclaration: check_import_declaration(static_cast<ast::ImportDeclaration *>(node)); return VOID_TYPE;

  case ast::NodeKind::VariableDeclaration: check_variable_declaration(static_cast<ast::VariableDeclaration *>(node)); return VOID_TYPE;

  case ast::NodeKind::FunctionDeclaration: check_function_declaration(static_cast<ast::FunctionDeclaration *>(node)); return VOID_TYPE;

  case ast::NodeKind::StructDeclaration: check_struct_declaration(static_cast<ast::StructDeclaration *>(node)); return VOID_TYPE;

  case ast::NodeKind::ModuleInitDeclaration: check_module_init_declaration(static_cast<ast::ModuleInitDeclaration *>(node)); return VOID_TYPE;

  case ast::NodeKind::ReturnStatement: check_return_statement(static_cast<ast::ReturnStatement *>(node)); return VOID_TYPE;

  case ast::NodeKind::NumberLiteral: return check_number_literal(static_cast<ast::NumberLiteral *>(node), expected);

  case ast::NodeKind::StringLiteral: return check_string_literal(static_cast<ast::StringLiteral *>(node), expected);

  case ast::NodeKind::BooleanLiteral: return check_boolean_literal(static_cast<ast::BoolLiteral *>(node), expected);

  case ast::NodeKind::ArrayLiteral: return check_array_literal(static_cast<ast::ArrayLiteral *>(node), expected);

  case ast::NodeKind::StructLiteral: return check_struct_literal(static_cast<ast::StructLiteral *>(node), expected);

  case ast::NodeKind::NamedPattern: return check_name_pattern(static_cast<ast::NamedPattern *>(node), expected);

  case ast::NodeKind::BlockExpression: return check_block_expression(static_cast<ast::BlockExpression *>(node), expected);

  case ast::NodeKind::IfExpression: return check_if_expression(static_cast<ast::IfExpression *>(node), expected);

  case ast::NodeKind::BinaryExpression: return check_binary_expression(static_cast<ast::BinaryExpression *>(node), expected);

  default: error(node, "unsupported node in type checker: " + std::string(ast::node_kind_name(node->kind))); return TypeId::invalid();
  }
}

} // namespace celestia::semantic