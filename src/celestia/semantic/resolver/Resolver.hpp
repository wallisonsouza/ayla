#pragma once

#include "celestia/ast/ASTFwd.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/resolver/ContextStack.hpp"

namespace celestia::semantic {
struct ResolverContext {
  Compiler &compiler;
  CompilationUnit &unit;

  ContextStack<ScopeId> stack;

  CompilerEnvironment &get_env() const { return compiler.environment(); }

  ResolverContext(Compiler &compiler, CompilationUnit &unit) : compiler(compiler), unit(unit), stack(ScopeId::invalid()) {}
};

struct Resolver {

public:
  Resolver(Compiler &compiler, CompilationUnit &unit);
  void resolve_root(ast::RootNode *node);

private:
  ResolverContext context;

  void resolve_node(ast::Node *node);
  SymbolId require_symbol(ast::NameNode *name, ScopeId scope_id);
  void resolve_named_type(ast::NamedType *node);
  void resolve_enum_variant(ast::EnumVariant *node);
  void resolve_enum_declaration(ast::EnumDeclaration *node);
  void resolve_generic_type(ast::GenericType *node);
  void resolve_function_type(ast::FunctionType *node);

  void function_call(ast::CallExpressionNode *node);
  void assignment(ast::AssignmentExpressionNode *node);

  void array_literal(ast::ArrayLiteral *node);
  void object_literal(ast::ObjectLiteral *node);
  void struct_literal(ast::StructLiteral *node);
  void index_access(ast::IndexAccessExpressionNode *node);
  void member_access(ast::MemberAccessExpressionNode *node);

  void resolve_identifier_expression(ast::IdentifierExpressionNode *node);
  void binary_expression(ast::BinaryExpressionNode *node);
  void unary_expression(ast::UnaryExpressionNode *node);
  void resolve_if_expression(ast::IfExpression *node);

  void while_expression(ast::WhileExpression *node);

  void resolve_variable_declaration(ast::VariableDeclaration *node);
  void resolve_module_init_declaration(ast::ModuleInitDeclaration *node);

  void resolve_generic_parameter(ast::GenericParameter *node);
  void resolve_struct_declaration(ast::StructDeclaration *node);
  void resolve_field_declaration(ast::FieldDeclaration *node);
  void resolve_function_declaration(ast::FunctionDeclaration *node);
  void resolve_module_declaration(ast::ModuleDeclaration *node);
  void resolve_capability_declaration(ast::CapabilityDeclaration *node);
  void resolve_impl_declaration(ast::ImplDeclaration *node);
  void resolve_type_declaration(ast::TypeDeclaration *node);
  void resolve_block_expression(ast::BlockExpression *node);
  void declare_generics(const std::vector<ast::Identifier *> &parameters);
  void return_statement(ast::ReturnStatement *node);

  void resolve_import_declaration(ast::ImportDeclaration *node);
  void expression_statement(ast::ExpressionStatement *node);
  void named_pattern(ast::NamedPattern *pattern);

  void diagnostic() {}
};
} // namespace celestia::semantic