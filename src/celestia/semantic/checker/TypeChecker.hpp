#pragma once

#include "celestia/ast/ASTFwd.hpp"
#include "celestia/semantic/SemanticContext.hpp"
#include "celestia/semantic/checker/TypeSystem.hpp"

namespace celestia::semantic {

struct ExpectedType {
  TypeId type = TypeId::invalid();
  const ast::Node *node = nullptr;
  SourceSlice slice;

  [[nodiscard]] bool is_valid() const { return type.is_valid(); }

  static ExpectedType none() { return {}; }
};

class TypeChecker {
public:
  explicit TypeChecker(SemanticContext &context);

  TypeId check(ast::Node *node, ExpectedType expected = ExpectedType::none());

  TypeId infer(ast::Node *node);

  TypeId resolve_type(ast::Type *node);

private:
  SemanticContext &context;

  TypeSystem type_system;

  TypeId current_function = TypeId::invalid();

  TypeId build_type(ast::Node *node);
  TypeId build_function_type(ast::FunctionDeclaration *node);
  TypeId build_struct_type(ast::StructDeclaration *node);

  // Expression

  TypeId infer_number_literal(ast::NumberLiteral *node);
  TypeId infer_string_literal(ast::StringLiteral *node);
  TypeId infer_boolean_literal(ast::BoolLiteral *node);
  TypeId infer_array_literal(ast::ArrayLiteral *node);
  TypeId infer_struct_literal(ast::StructLiteral *node);
  TypeId infer_name_pattern(ast::NamedPattern *pattern);
  TypeId infer_identifier_expression(ast::IdentifierExpressionNode *node);

  TypeId infer_block_expression(ast::BlockExpression *node);
  TypeId infer_if_expression(ast::IfExpression *node);
  TypeId infer_binary_expression(ast::BinaryExpression *node);
  TypeId infer_while_expression(ast::WhileExpression *node);

  TypeId check_number_literal(ast::NumberLiteral *node, ExpectedType expected);
  TypeId check_string_literal(ast::StringLiteral *node, ExpectedType expected);
  TypeId check_boolean_literal(ast::BoolLiteral *node, ExpectedType expected);

  TypeId check_array_elements(ast::ArrayLiteral *node);
  TypeId check_array_literal(ast::ArrayLiteral *node, ExpectedType expected);
  TypeId check_struct_literal(ast::StructLiteral *node, ExpectedType expected);
  TypeId check_name_pattern(ast::NamedPattern *pattern, ExpectedType expected);

  TypeId check_block_expression(ast::BlockExpression *node, ExpectedType expected);
  TypeId check_if_expression(ast::IfExpression *pattern, ExpectedType expected);
  TypeId check_binary_expression(ast::BinaryExpression *node, ExpectedType expected);
  // --------------------------------------------------
  // Types
  // --------------------------------------------------

  TypeId create_function_type(ast::FunctionDeclaration *node);

  TypeId resolve_generic_type(ast::GenericType *node);

  TypeId resolve_named_type(ast::NamedType *node);
  TypeId resolve_pattern_type(ast::PatternNode *pattern);

  TypeId check_generic_instance_type(ast::GenericType *node);

  void unary_expression(ast::UnaryExpressionNode *node);
  void assignment(ast::AssignmentExpressionNode *node);
  void function_call(ast::CallExpressionNode *node);
  void member_access(ast::MemberAccessExpressionNode *node);
  void index_access(ast::IndexAccessExpressionNode *node);
  void identifier(ast::IdentifierExpressionNode *node);

  // --------------------------------------------------
  // Statements
  // --------------------------------------------------

  void check_expression_statement(ast::ExpressionStatement *node);
  void check_return_statement(ast::ReturnStatement *node);
  void check_while_expression(ast::WhileExpression *node);
  // --------------------------------------------------
  // Declarations
  // --------------------------------------------------
  TypeId check_root(ast::RootNode *node);
  TypeId check_import_declaration(ast::ImportDeclaration *node);
  TypeId check_variable_declaration(ast::VariableDeclaration *node);
  TypeId check_function_declaration(ast::FunctionDeclaration *node);
  TypeId check_struct_declaration(ast::StructDeclaration *node);
  TypeId check_module_declaration(ast::ModuleDeclaration *node);
  TypeId check_module_init_declaration(ast::ModuleInitDeclaration *node);

  void error(ast::Node *node, std::string message) { std::cerr << message << '\n'; }
};

} // namespace celestia::semantic