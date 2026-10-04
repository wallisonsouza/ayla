#pragma once

#include "celestia/ast/ASTFwd.hpp"
#include "celestia/semantic/checker/TypeSystem.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"

namespace celestia::semantic {

struct TypeCheckerContext {
  Compiler &compiler;
  CompilationUnit &unit;

  TypeCheckerContext(Compiler &compiler, CompilationUnit &unit) : compiler(compiler), unit(unit) {}

  CompilerEnvironment &env() { return compiler.environment(); }

  const CompilerEnvironment &env() const { return compiler.environment(); }

  const SourceSlice &diagnostic_slice() {

    if (auto *token = unit.tokens.current()) return token->slice;

    if (auto *token = unit.tokens.previous()) return token->slice;

    static SourceSlice empty{};
    return empty;
  }
};

class TypeChecker {
public:
  explicit TypeChecker(TypeCheckerContext &context);

  bool check(ast::Node *node, TypeId expected = TypeId::invalid());

  // Expressions without an expected type.
  TypeId infer(ast::Node *node);

private:
  TypeCheckerContext &context;

  TypeSystem type_system;

  TypeId current_function = TypeId::invalid();

  // Expression

  TypeId infer_number_literal(ast::NumberLiteral *node);
  TypeId infer_string_literal(ast::StringLiteral *node);
  TypeId infer_boolean_literal(ast::BoolLiteral *node);
  TypeId infer_array_literal(ast::ArrayLiteral *node);
  TypeId infer_struct_literal(ast::StructLiteral *node);
  TypeId infer_name_pattern(ast::NamedPattern *pattern);

  TypeId infer_block_expression(ast::BlockExpression *node);
  TypeId infer_if_expression(ast::IfExpression *node);
  TypeId infer_binary_expression(ast::BinaryExpressionNode *node);
  TypeId infer_while_expression(ast::WhileExpression *node);

  bool check_number_literal(ast::NumberLiteral *node, TypeId expected);
  bool check_string_literal(ast::StringLiteral *node, TypeId expected);
  bool check_boolean_literal(ast::BoolLiteral *node, TypeId expected);
  bool check_array_literal(ast::ArrayLiteral *node, TypeId expected);
  bool check_struct_literal(ast::StructLiteral *node, TypeId expected);
  bool check_name_pattern(ast::NamedPattern *pattern, TypeId expected);

  bool check_block_expression(ast::BlockExpression *node, TypeId expected);
  bool check_if_expression(ast::IfExpression *pattern, TypeId expected);

  // --------------------------------------------------
  // Types
  // --------------------------------------------------

  TypeId create_function_type(ast::FunctionDeclaration *node);

  TypeId infer_generic_type(ast::GenericType *node);

  TypeId infer_named_type(ast::NamedType *node);

  TypeId check_generic_instance_type(ast::GenericType *node);

  //   TypeId type_from_node(ast::Type *node);

  //   TypeId TypeChecker::type_from_node(ast::Type *node) {
  //   if (!node) return TypeId::invalid();

  //   switch (node->kind) {

  //   case ast::NodeKind::NamedType: return check_named_type(static_cast<ast::NamedType *>(node));

  //   case ast::NodeKind::GenericType: return check_generic_type(static_cast<ast::GenericType *>(node));

  //   default: error(node, "unsupported type node"); return TypeId::invalid();
  //   }
  // }

  // --------------------------------------------------
  // Functions
  // --------------------------------------------------

  bool check_function_parameters(ast::FunctionDeclaration *node, FunctionType &function_type);

  bool check_function_return_type(ast::FunctionDeclaration *node, FunctionType &function_type);

  void check_function_body(ast::FunctionDeclaration *node, TypeId function_type);

  // --------------------------------------------------
  // Expressions
  // --------------------------------------------------

  void check_binary_expression(ast::BinaryExpressionNode *node);
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
  void check_root(ast::RootNode *node);
  void check_import_declaration(ast::ImportDeclaration *node);
  void check_variable_declaration(ast::VariableDeclaration *node);
  void check_function_declaration(ast::FunctionDeclaration *node);
  void check_struct_declaration(ast::StructDeclaration *node);
  void check_module_declaration(ast::ModuleDeclaration *node);
  void check_module_init_declaration(ast::ModuleInitDeclaration *node);

  void error(ast::Node *node, std::string message) { std::cerr << message << '\n'; }
};

} // namespace celestia::semantic