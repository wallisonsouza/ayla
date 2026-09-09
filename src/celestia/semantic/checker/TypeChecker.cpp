#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

TypeChecker::TypeChecker(TypeCheckerContext &context) : context(context), dispatcher() {
  bind_literals();
  bind_expressions();
  bind_statements();
  bind_declarations();
  // bind_types();
}

bool TypeChecker::is_same_type(TypeId a, TypeId b) const {
  if (!a.is_valid() || !b.is_valid()) return false;

  return a == b;
}

bool TypeChecker::is_assignable(TypeId target, TypeId source) const {

  if (!target.is_valid() || !source.is_valid()) return false;

  // Mesmo tipo.
  if (is_same_type(target, source)) return true;

  const auto &target_type = context.env().types.get(target);

  const auto &source_type = context.env().types.get(source);

  // Primitive
  if (target_type.kind == TypeKind::Primitive && source_type.kind == TypeKind::Primitive) {

    const auto &t = static_cast<const PrimitiveType &>(target_type);

    const auto &s = static_cast<const PrimitiveType &>(source_type);

    return t.primitive == s.primitive;
  }

  // Generic
  if (target_type.kind == TypeKind::GenericInstance && source_type.kind == TypeKind::GenericInstance) {

    const auto &t = static_cast<const GenericInstanceType &>(target_type);

    const auto &s = static_cast<const GenericInstanceType &>(source_type);

    if (t.constructor != s.constructor) return false;

    if (t.arguments.size() != s.arguments.size()) return false;

    for (size_t i = 0; i < t.arguments.size(); ++i) {

      if (!is_assignable(t.arguments[i], s.arguments[i])) { return false; }
    }

    return true;
  }

  return false;
}

void TypeChecker::bind_declarations() {

  dispatcher.bind<ast::ModuleDeclaration, &TypeChecker::check_module_declaration>();

  dispatcher.bind<ast::VariableDeclaration, &TypeChecker::check_variable_declaration>();

  dispatcher.bind<ast::FunctionDeclaration, &TypeChecker::check_function_declaration>();

  dispatcher.bind<ast::StructDeclaration, &TypeChecker::check_struct_declaration>();

  dispatcher.bind<ast::ModuleInitDeclaration, &TypeChecker::check_module_init_declaration>();
}

void TypeChecker::bind_expressions() {

  dispatcher.bind<ast::BinaryExpressionNode, &TypeChecker::binary_expression>();

  // Adicione outros binds de expressões conforme necessário.
}

void TypeChecker::bind_statements() {

  dispatcher.bind<ast::BlockStatement, &TypeChecker::check_block_statement>();

  dispatcher.bind<ast::ReturnStatement, &TypeChecker::check_return_statement>();
}

void TypeChecker::bind_literals() {

  dispatcher.bind<ast::NumberLiteralNode, &TypeChecker::number_literal>();

  dispatcher.bind<ast::StringLiteralNode, &TypeChecker::string_literal>();

  dispatcher.bind<ast::BoolLiteralNode, &TypeChecker::boolean_literal>();

  dispatcher.bind<ast::ArrayLiteralNode, &TypeChecker::array_literal>();

  dispatcher.bind<ast::StructLiteralNode, &TypeChecker::check_struct_literal>();
}

void TypeChecker::check(ast::Node *node) {
  if (!node) return;

  if ( dispatcher.dispatch(this, node)== DispatchResult::NotHandled) { std::cerr << "Checker: no handler for NodeKind: " << celestia::ast::node_kind_name(node->kind) << '\n'; }
}

TypeId TypeChecker::type_from_node(ast::TypeNode *node) {
  if (!node) return TypeId::invalid();

  switch (node->kind) {

  case ast::NodeKind::NamedType: return check_named_type(static_cast<ast::NamedType *>(node));

  case ast::NodeKind::GenericType: return check_generic_type(static_cast<ast::GenericTypeNode *>(node));

  default: error(node, "unsupported type node"); return TypeId::invalid();
  }
}

} // namespace celestia::semantic