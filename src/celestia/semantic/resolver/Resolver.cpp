#include "Resolver.hpp"

namespace celestia::semantic {

Resolver::Resolver(Compiler &compiler, CompilationUnit &unit) : context(compiler, unit) {
  bind_literals();
  bind_expressions();
  bind_statements();
  bind_declarations();
  bind_types();
}

void Resolver::bind_literals() {
  dispatcher.bind<ast::StructLiteralNode, &Resolver::struct_literal>();

  dispatcher.bind<ast::ArrayLiteralNode, &Resolver::array_literal>();

  dispatcher.bind<ast::ObjectLiteralNode, &Resolver::object_literal>();
  dispatcher.bind<ast::NamedPattern, &Resolver::named_pattern>();
}

void Resolver::bind_expressions() {
  dispatcher.bind<ast::BinaryExpressionNode, &Resolver::binary_expression>();

  dispatcher.bind<ast::UnaryExpressionNode, &Resolver::unary_expression>();

  dispatcher.bind<ast::AssignmentExpressionNode, &Resolver::assignment>();

  dispatcher.bind<ast::CallExpressionNode, &Resolver::function_call>();

  dispatcher.bind<ast::MemberAccessExpressionNode, &Resolver::member_access>();

  dispatcher.bind<ast::IndexAccessExpressionNode, &Resolver::index_access>();

  dispatcher.bind<ast::IdentifierExpressionNode, &Resolver::identifier>();
}

void Resolver::bind_statements() {
  dispatcher.bind<ast::IfStatement, &Resolver::if_statement>();

  dispatcher.bind<ast::WhileStatement, &Resolver::while_statement>();

  dispatcher.bind<ast::BlockStatement, &Resolver::block_statement>();

  dispatcher.bind<ast::ExpressionStatement, &Resolver::expression_statement>();

  dispatcher.bind<ast::ReturnStatement, &Resolver::return_statement>();
}

void Resolver::bind_declarations() {

  dispatcher.bind<ast::TypeDeclaration, &Resolver::resolve_type_declaration>();

  dispatcher.bind<ast::VariableDeclaration, &Resolver::resolve_variable_declaration>();

  dispatcher.bind<ast::FunctionDeclaration, &Resolver::resolve_function_declaration>();

  dispatcher.bind<ast::ModuleDeclaration, &Resolver::resolve_module_declaration>();

  dispatcher.bind<ast::ImportDeclaration, &Resolver::resolve_import_declaration>();

  dispatcher.bind<ast::CapabilityDeclaration, &Resolver::resolve_capability_declaration>();

  dispatcher.bind<ast::ImplDeclaration, &Resolver::resolve_impl_declaration>();

  dispatcher.bind<ast::FieldDeclaration, &Resolver::resolve_field_declaration>();

  dispatcher.bind<ast::StructDeclaration, &Resolver::resolve_struct_declaration>();

  dispatcher.bind<ast::ModuleInitDeclaration, &Resolver::resolve_module_init_declaration>();
}

void Resolver::bind_types() {
  dispatcher.bind<ast::NamedType, &Resolver::resolve_named_type>();

  dispatcher.bind<ast::GenericTypeNode, &Resolver::resolve_generic_type>();

  dispatcher.bind<ast::FunctionType, &Resolver::resolve_function_type>();
}

void Resolver::resolve_root(ast::RootNode *node) {

  if (!node) return;

  for (auto *module : node->modules) {

    if (!module) continue;

    resolve_node(module);
  }
}

void Resolver::resolve_node(ast::Node *node) {
  if (!node) { return; }

  auto result = dispatcher.dispatch(this, node);

  if (result == DispatchResult::NotHandled) { std::cerr << "Resolver: no handler for NodeKind: " << celestia::ast::node_kind_name(node->kind) << '\n'; }
}

} // namespace celestia::semantic