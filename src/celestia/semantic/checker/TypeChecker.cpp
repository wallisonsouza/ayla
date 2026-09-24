#include "celestia/semantic/checker/TypeChecker.hpp"
namespace celestia::semantic {

TypeChecker::TypeChecker(TypeCheckerContext &context) : context(context) {}

bool TypeChecker::check(ast::Node *node, TypeId expected) {

  if (!node) return false;

  switch (node->kind) {

  case ast::NodeKind::Root: check_root(static_cast<ast::RootNode *>(node)); return true;

  case ast::NodeKind::ModuleDeclaration: check_module_declaration(static_cast<ast::ModuleDeclaration *>(node)); return true;

  case ast::NodeKind::VariableDeclaration: check_variable_declaration(static_cast<ast::VariableDeclaration *>(node)); return true;

  case ast::NodeKind::FunctionDeclaration: check_function_declaration(static_cast<ast::FunctionDeclaration *>(node)); return true;

  case ast::NodeKind::StructDeclaration: check_struct_declaration(static_cast<ast::StructDeclaration *>(node)); return true;

  case ast::NodeKind::ModuleInitDeclaration: check_module_init_declaration(static_cast<ast::ModuleInitDeclaration *>(node)); return true;

  case ast::NodeKind::BlockStatement: check_block_statement(static_cast<ast::BlockStatement *>(node)); return true;

  case ast::NodeKind::ReturnStatement: check_return_statement(static_cast<ast::ReturnStatement *>(node)); return true;

  case ast::NodeKind::NumberLiteral: return check_number_literal(static_cast<ast::NumberLiteralNode *>(node), expected);

  case ast::NodeKind::StringLiteral: return check_string_literal(static_cast<ast::StringLiteralNode *>(node), expected);

  case ast::NodeKind::BooleanLiteral: return check_boolean_literal(static_cast<ast::BoolLiteralNode *>(node), expected);

  case ast::NodeKind::ArrayLiteral: return check_array_literal(static_cast<ast::ArrayLiteralNode *>(node), expected);

  case ast::NodeKind::StructLiteral: return check_struct_literal(static_cast<ast::StructLiteralNode *>(node), expected);

  case ast::NodeKind::NamedPattern: return check_name_pattern(static_cast<ast::NamedPattern *>(node), expected);

  default: error(node, "unsupported node in type checker"); return false;
  }
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

} // namespace celestia::semantic