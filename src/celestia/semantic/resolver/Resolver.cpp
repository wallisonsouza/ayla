#include "Resolver.hpp"
#include "celestia/ast/NodeCast.hpp"

namespace celestia::semantic {

Resolver::Resolver(Compiler &compiler, CompilationUnit &unit) : context(compiler, unit) {}

void Resolver::resolve_node(ast::Node *node) {

  if (!node) return;

  switch (node->kind) {

  // Names
  case ast::NodeKind::Identifier:
  case ast::NodeKind::QualifiedName: break;

  // Literals
  case ast::NodeKind::StructLiteral: struct_literal(ast::as<ast::StructLiteralNode>(node)); break;

  case ast::NodeKind::ArrayLiteral: array_literal(ast::as<ast::ArrayLiteralNode>(node)); break;

  case ast::NodeKind::ObjectLiteral: object_literal(ast::as<ast::ObjectLiteralNode>(node)); break;

  case ast::NodeKind::NumberLiteral:
  case ast::NodeKind::StringLiteral:
  case ast::NodeKind::BooleanLiteral:
  case ast::NodeKind::NullLiteral: break;

  // Expressions
  case ast::NodeKind::BinaryExpression: binary_expression(ast::as<ast::BinaryExpressionNode>(node)); break;

  case ast::NodeKind::UnaryExpression: unary_expression(ast::as<ast::UnaryExpressionNode>(node)); break;

  case ast::NodeKind::Assignment: assignment(ast::as<ast::AssignmentExpressionNode>(node)); break;

  case ast::NodeKind::Call: function_call(ast::as<ast::CallExpressionNode>(node)); break;

  case ast::NodeKind::MemberAccess: member_access(ast::as<ast::MemberAccessExpressionNode>(node)); break;

  case ast::NodeKind::IndexAccess: index_access(ast::as<ast::IndexAccessExpressionNode>(node)); break;

  case ast::NodeKind::IdentifierExpression: resolve_identifier_expression(ast::as<ast::IdentifierExpressionNode>(node)); break;

  // Statements
  case ast::NodeKind::IfStatement: if_statement(ast::as<ast::IfStatement>(node)); break;

  case ast::NodeKind::WhileStatement: while_statement(ast::as<ast::WhileStatement>(node)); break;

  case ast::NodeKind::BlockStatement: block_statement(ast::as<ast::BlockStatement>(node)); break;

  case ast::NodeKind::ExpressionStatement: expression_statement(ast::as<ast::ExpressionStatement>(node)); break;

  case ast::NodeKind::ReturnStatement: return_statement(ast::as<ast::ReturnStatement>(node)); break;

  // Patterns
  case ast::NodeKind::NamedPattern: named_pattern(ast::as<ast::NamedPattern>(node)); break;

  // Declarations
  case ast::NodeKind::EnumDeclaration: resolve_enum_declaration(ast::as<ast::EnumDeclaration>(node)); break;

  case ast::NodeKind::EnumVariant: resolve_enum_variant(ast::as<ast::EnumVariant>(node)); break;

  case ast::NodeKind::TypeDeclaration: resolve_type_declaration(ast::as<ast::TypeDeclaration>(node)); break;

  case ast::NodeKind::VariableDeclaration: resolve_variable_declaration(ast::as<ast::VariableDeclaration>(node)); break;

  case ast::NodeKind::FunctionDeclaration: resolve_function_declaration(ast::as<ast::FunctionDeclaration>(node)); break;

  case ast::NodeKind::ModuleDeclaration: resolve_module_declaration(ast::as<ast::ModuleDeclaration>(node)); break;

  case ast::NodeKind::ImportDeclaration: resolve_import_declaration(ast::as<ast::ImportDeclaration>(node)); break;

  case ast::NodeKind::CapabilityDeclaration: resolve_capability_declaration(ast::as<ast::CapabilityDeclaration>(node)); break;

  case ast::NodeKind::ImplementationDeclaration: resolve_impl_declaration(ast::as<ast::ImplDeclaration>(node)); break;

  case ast::NodeKind::FieldDeclaration: resolve_field_declaration(ast::as<ast::FieldDeclaration>(node)); break;

  case ast::NodeKind::StructDeclaration: resolve_struct_declaration(ast::as<ast::StructDeclaration>(node)); break;

  case ast::NodeKind::ModuleInitDeclaration: resolve_module_init_declaration(ast::as<ast::ModuleInitDeclaration>(node)); break;

  // Types
  case ast::NodeKind::NamedType: resolve_named_type(ast::as<ast::NamedType>(node)); break;

  case ast::NodeKind::GenericType: resolve_generic_type(ast::as<ast::GenericType>(node)); break;

  case ast::NodeKind::GenericParameter: resolve_generic_parameter(ast::as<ast::GenericParameter>(node)); break;

  case ast::NodeKind::FunctionType: resolve_function_type(ast::as<ast::FunctionType>(node)); break;

  default: std::cerr << "Resolver: unhandled NodeKind: " << ast::node_kind_name(node->kind) << '\n'; break;
  }
}

void Resolver::resolve_root(ast::RootNode *node) {

  if (!node) return;

  for (auto *module : node->modules) {

    if (!module) continue;

    resolve_node(module);
  }
}

} // namespace celestia::semantic