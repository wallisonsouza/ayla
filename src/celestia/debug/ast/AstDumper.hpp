#pragma once

#include <iostream>
#include <ostream>

#include "celestia/ast/AstDispacher.hpp"
#include "celestia/ast/Node.hpp"
#include "celestia/ast/RootNode.hpp"
#include "celestia/ast/declarations/CapabilityDeclaration.hpp"
#include "celestia/ast/declarations/EnumDeclaration.hpp"
#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/ast/declarations/ImportDeclaration.hpp"
#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
#include "celestia/ast/expressions/AssignmentExpression.hpp"
#include "celestia/ast/expressions/BinaryExpressionNode.hpp"
#include "celestia/ast/expressions/CallExpressionNode.hpp"
#include "celestia/ast/expressions/IdentifierExpressionNode.hpp"
#include "celestia/ast/expressions/IndexAcessExpressionNode.hpp"
#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/ast/expressions/MemberAccessExpressionNode.hpp"

#include "celestia/ast/expressions/UnaryExpressionNode.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/ast/statements/BlockStatementNode.hpp"
#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/ast/statements/IfStatementNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"
#include "celestia/ast/statements/WhileStatementNode.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/debug/ast/DumpContext.hpp"

namespace celestia::debug {
class AstDumper {

public:
  AstDumper(std::ostream &out = std::cout) : dispatcher(), context(out, [this](const celestia::ast::Node *node) { dispatch(node); }) { register_handlers(); }

public:
  void dump(const ast::RootNode *root) { dispatch(root); }

  void dispatch(const celestia::ast::Node *node) {
    if (!node) return;

    if (dispatcher.dispatch(this, node) == DispatchResult::NotHandled) { std::cerr << "AstDumper: no handler for NodeKind: " << celestia::ast::node_kind_name(node->kind) << '\n'; }
  }

private:
  void register_handlers() {
    // Literals
    dispatcher.bind<ast::NumberLiteralNode, &AstDumper::dump_number_literal>();

    dispatcher.bind<ast::StringLiteralNode, &AstDumper::dump_string_literal>();

    dispatcher.bind<ast::BoolLiteralNode, &AstDumper::dump_bool_literal>();

    dispatcher.bind<ast::NullLiteralNode, &AstDumper::dump_null_literal>();

    // Names
    dispatcher.bind<ast::IdentifierNode, &AstDumper::dump_identifier>();

    dispatcher.bind<ast::QualifiedNameNode, &AstDumper::dump_qualified_name>();

    // Expressions
    dispatcher.bind<ast::IdentifierExpressionNode, &AstDumper::dump_identifier_expression>();

    dispatcher.bind<ast::BinaryExpressionNode, &AstDumper::dump_binary_expression>();

    dispatcher.bind<ast::UnaryExpressionNode, &AstDumper::dump_unary_expression>();

    dispatcher.bind<ast::CallExpressionNode, &AstDumper::dump_call_expression>();

    dispatcher.bind<ast::MemberAccessExpressionNode, &AstDumper::dump_member_acess_expression>();

    dispatcher.bind<ast::IndexAccessExpressionNode, &AstDumper::dump_index_acess_expression>();

    dispatcher.bind<ast::AssignmentExpressionNode, &AstDumper::dump_assignment_expression>();

    // Statements
    dispatcher.bind<ast::ExpressionStatement, &AstDumper::dump_expression_statement>();

    dispatcher.bind<ast::BlockStatement, &AstDumper::dump_block_statement>();

    dispatcher.bind<ast::IfStatement, &AstDumper::dump_if_statement>();

    dispatcher.bind<ast::WhileStatement, &AstDumper::dump_while_statement>();

    dispatcher.bind<ast::ReturnStatement, &AstDumper::dump_return_statement>();

    dispatcher.bind<ast::ImportDeclaration, &AstDumper::dump_import_statement>();

    // Declarations
    dispatcher.bind<ast::VariableDeclaration, &AstDumper::dump_variable_declaration>();
    dispatcher.bind<ast::EnumVariant, &AstDumper::dump_enum_variant>();
    dispatcher.bind<ast::EnumDeclaration, &AstDumper::dump_enum_declaration>();

    dispatcher.bind<ast::FunctionDeclaration, &AstDumper::dump_function_declaration>();

    dispatcher.bind<ast::ModuleDeclaration, &AstDumper::dump_module_declaration>();

    dispatcher.bind<ast::ModuleInitDeclaration, &AstDumper::dump_module_init>();

    dispatcher.bind<ast::CapabilityDeclaration, &AstDumper::dump_capability_declaration>();

    dispatcher.bind<ast::ImplDeclaration, &AstDumper::dump_impl_declaration>();

    dispatcher.bind<ast::FieldDeclaration, &AstDumper::dump_field_declaration>();

    dispatcher.bind<ast::StructDeclaration, &AstDumper::dump_struct_declaration>();

    // Other nodes
    dispatcher.bind<ast::NamedPattern, &AstDumper::dump_named_pattern>();

    dispatcher.bind<ast::TypeNode, &AstDumper::dump_type>();

    dispatcher.bind<ast::ObjectLiteralNode, &AstDumper::dump_object_literal>();

    dispatcher.bind<ast::ObjectFieldNode, &AstDumper::dump_object_field>();

    dispatcher.bind<ast::ArrayLiteralNode, &AstDumper::dump_array_literal>();

    dispatcher.bind<ast::StructFieldInitializerNode, &AstDumper::dump_struct_field>();

    dispatcher.bind<ast::StructLiteralNode, &AstDumper::dump_struct_literal>();
    dispatcher.bind<ast::RootNode, &AstDumper::dump_root>();

    // Types
    dispatcher.bind<ast::NamedType, &AstDumper::dump_named_type>();

    dispatcher.bind<ast::GenericTypeNode, &AstDumper::dump_generic_type>();
    dispatcher.bind<ast::TypeDeclaration, &AstDumper::dump_type_declaration>();
    dispatcher.bind<ast::GenericParameter, &AstDumper::dump_generic>();
    dispatcher.bind<ast::GenericIdentifierNode, &AstDumper::dump_generic_name>();
  }

  void dump_root(const ast::RootNode *node);
  void dump_generic_name(const ast::GenericIdentifierNode *node);
  void dump_impl_declaration(const ast::ImplDeclaration *node);
  void dump_number_literal(const ast::NumberLiteralNode *node);
  void dump_string_literal(const ast::StringLiteralNode *node);
  void dump_struct_field(const ast::StructFieldInitializerNode *node);
  void dump_struct_literal(const ast::StructLiteralNode *node);
  void dump_bool_literal(const ast::BoolLiteralNode *node);
  void dump_null_literal(const ast::NullLiteralNode *node);
  void dump_identifier(const ast::IdentifierNode *node);
  void dump_qualified_name(const ast::QualifiedNameNode *node);
  void dump_generic(const ast::GenericParameter *node);

  // Expressions
  void dump_identifier_expression(const ast::IdentifierExpressionNode *node);
  void dump_binary_expression(const ast::BinaryExpressionNode *node);
  void dump_unary_expression(const ast::UnaryExpressionNode *node);
  void dump_call_expression(const ast::CallExpressionNode *node);
  void dump_member_acess_expression(const ast::MemberAccessExpressionNode *node);
  void dump_index_acess_expression(const ast::IndexAccessExpressionNode *node);
  void dump_assignment_expression(const ast::AssignmentExpressionNode *node);

  // Statements
  void dump_expression_statement(const ast::ExpressionStatement *node);
  void dump_block_statement(const ast::BlockStatement *node);
  void dump_if_statement(const ast::IfStatement *node);
  void dump_while_statement(const ast::WhileStatement *node);
  void dump_return_statement(const ast::ReturnStatement *node);
  void dump_import_statement(const ast::ImportDeclaration *node);

  // Declarations
  void dump_enum_variant(const ast::EnumVariant *node);
  void dump_enum_declaration(const ast::EnumDeclaration *node);
  void dump_variable_declaration(const ast::VariableDeclaration *node);
  void dump_function_declaration(const ast::FunctionDeclaration *node);
  void dump_module_declaration(const ast::ModuleDeclaration *node);
  void dump_module_init(const ast::ModuleInitDeclaration *node);
  void dump_capability_declaration(const ast::CapabilityDeclaration *node);

  // Other nodes
  void dump_named_pattern(const ast::NamedPattern *node);
  void dump_type(const ast::TypeNode *node);
  void dump_object_literal(const ast::ObjectLiteralNode *node);
  void dump_object_field(const ast::ObjectFieldNode *node);
  void dump_array_literal(const ast::ArrayLiteralNode *node);
  void dump_field_declaration(const ast::FieldDeclaration *node);
  void dump_struct_declaration(const ast::StructDeclaration *node);
  void dump_type_declaration(const ast::TypeDeclaration *node);
  void dump_named_type(const ast::NamedType *node);
  void dump_generic_type(const ast::GenericTypeNode *node);
  void dump_name(const ast::NameNode *node);

private:
  AstDispatcher<AstDumper, const celestia::ast::Node> dispatcher;
  DumpContext context;
};

} // namespace celestia::debug
