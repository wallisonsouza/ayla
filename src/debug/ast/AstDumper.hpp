#pragma once

#include <iostream>
#include <ostream>

#include "celestia/ast/AstDispacher.hpp"
#include "celestia/ast/Node.hpp"
#include "celestia/ast/RootNode.hpp"
#include "celestia/ast/declaration/CapabilityDeclaration.hpp"
#include "celestia/ast/declaration/EnumDeclaration.hpp"
#include "celestia/ast/declaration/FunctionDeclaration.hpp"
#include "celestia/ast/declaration/ImplementationDeclaration.hpp"
#include "celestia/ast/declaration/ImportDeclaration.hpp"
#include "celestia/ast/declaration/ModuleDeclaration.hpp"
#include "celestia/ast/declaration/StructDeclaration.hpp"
#include "celestia/ast/declaration/TypeDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"
#include "celestia/ast/expression/BoolLiteral.hpp"

#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "debug/ast/DumpContext.hpp"

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
    dispatcher.bind<ast::NumberLiteral, &AstDumper::dump_number_literal>();

    dispatcher.bind<ast::StringLiteral, &AstDumper::dump_string_literal>();

    dispatcher.bind<ast::BoolLiteral, &AstDumper::dump_bool_literal>();

    dispatcher.bind<ast::NullLiteral, &AstDumper::dump_null_literal>();

    // Names
    dispatcher.bind<ast::Identifier, &AstDumper::dump_identifier>();

    dispatcher.bind<ast::QualifiedName, &AstDumper::dump_qualified_name>();

    // Expressions
    dispatcher.bind<ast::MatchArm, &AstDumper::dump_match_arm>();

    dispatcher.bind<ast::MatchExpression, &AstDumper::dump_match_expression>();

    dispatcher.bind<ast::IdentifierExpressionNode, &AstDumper::dump_identifier_expression>();

    dispatcher.bind<ast::BinaryExpression, &AstDumper::dump_binary_expression>();

    dispatcher.bind<ast::UnaryExpressionNode, &AstDumper::dump_unary_expression>();

    dispatcher.bind<ast::CallExpressionNode, &AstDumper::dump_call_expression>();

    dispatcher.bind<ast::MemberAccessExpressionNode, &AstDumper::dump_member_acess_expression>();

    dispatcher.bind<ast::IndexAccessExpressionNode, &AstDumper::dump_index_acess_expression>();

    dispatcher.bind<ast::AssignmentExpressionNode, &AstDumper::dump_assignment_expression>();

    // Statements
    dispatcher.bind<ast::ExpressionStatement, &AstDumper::dump_expression_statement>();

    dispatcher.bind<ast::BlockExpression, &AstDumper::dump_block_expression>();

    dispatcher.bind<ast::IfExpression, &AstDumper::dump_if_expression>();

    dispatcher.bind<ast::WhileExpression, &AstDumper::dump_while_expression>();

    dispatcher.bind<ast::ReturnStatement, &AstDumper::dump_return_statement>();

    dispatcher.bind<ast::ImportDeclaration, &AstDumper::dump_import_statement>();

    // Declarations
    dispatcher.bind<ast::VariableDeclaration, &AstDumper::dump_variable_declaration>();

    dispatcher.bind<ast::EnumVariant, &AstDumper::dump_enum_variant>();

    dispatcher.bind<ast::EnumDeclaration, &AstDumper::dump_enum_declaration>();

    dispatcher.bind<ast::FunctionDeclaration, &AstDumper::dump_function_declaration>();

    dispatcher.bind<ast::ModuleDeclaration, &AstDumper::dump_module_declaration>();

    dispatcher.bind<ast::ModuleInitDeclaration, &AstDumper::dump_module_init_declaration>();

    dispatcher.bind<ast::CapabilityDeclaration, &AstDumper::dump_capability_declaration>();

    dispatcher.bind<ast::ImplDeclaration, &AstDumper::dump_impl_declaration>();

    dispatcher.bind<ast::FieldDeclaration, &AstDumper::dump_field_declaration>();

    dispatcher.bind<ast::StructDeclaration, &AstDumper::dump_struct_declaration>();

    // Other nodes
    dispatcher.bind<ast::NamedPattern, &AstDumper::dump_named_pattern>();
    dispatcher.bind<ast::EnumVariantPattern, &AstDumper::dump_enum_variant_pattern>();

    dispatcher.bind<ast::Type, &AstDumper::dump_type>();

    dispatcher.bind<ast::ObjectLiteral, &AstDumper::dump_object_literal>();

    dispatcher.bind<ast::ObjectField, &AstDumper::dump_object_field>();

    dispatcher.bind<ast::ArrayLiteral, &AstDumper::dump_array_literal>();

    dispatcher.bind<ast::StructField, &AstDumper::dump_struct_field>();

    dispatcher.bind<ast::StructLiteral, &AstDumper::dump_struct_literal>();
    dispatcher.bind<ast::RootNode, &AstDumper::dump_root>();

    // Types
    dispatcher.bind<ast::NamedType, &AstDumper::dump_named_type>();

    dispatcher.bind<ast::GenericType, &AstDumper::dump_generic_type>();
    dispatcher.bind<ast::TypeDeclaration, &AstDumper::dump_type_declaration>();
    dispatcher.bind<ast::GenericParameter, &AstDumper::dump_generic>();
    dispatcher.bind<ast::GenericName, &AstDumper::dump_generic_name>();
  }

  void dump_root(const ast::RootNode *node);
  void dump_generic_name(const ast::GenericName *node);
  void dump_impl_declaration(const ast::ImplDeclaration *node);
  void dump_number_literal(const ast::NumberLiteral *node);
  void dump_string_literal(const ast::StringLiteral *node);
  void dump_struct_field(const ast::StructField *node);
  void dump_struct_literal(const ast::StructLiteral *node);
  void dump_bool_literal(const ast::BoolLiteral *node);
  void dump_null_literal(const ast::NullLiteral *node);
  void dump_identifier(const ast::Identifier *node);
  void dump_qualified_name(const ast::QualifiedName *node);
  void dump_generic(const ast::GenericParameter *node);

  // Expressions
  void dump_match_arm(const ast::MatchArm *node);
  void dump_match_expression(const ast::MatchExpression *node);

  void dump_while_expression(const ast::WhileExpression *node);
  void dump_identifier_expression(const ast::IdentifierExpressionNode *node);
  void dump_binary_expression(const ast::BinaryExpression*node);
  void dump_unary_expression(const ast::UnaryExpressionNode *node);
  void dump_call_expression(const ast::CallExpressionNode *node);
  void dump_member_acess_expression(const ast::MemberAccessExpressionNode *node);
  void dump_index_acess_expression(const ast::IndexAccessExpressionNode *node);
  void dump_assignment_expression(const ast::AssignmentExpressionNode *node);

  // Statements
  void dump_expression_statement(const ast::ExpressionStatement *node);
  void dump_block_expression(const ast::BlockExpression *node);
  void dump_if_expression(const ast::IfExpression *node);

  void dump_return_statement(const ast::ReturnStatement *node);
  void dump_import_statement(const ast::ImportDeclaration *node);

  // Declarations
  void dump_enum_variant(const ast::EnumVariant *node);
  void dump_enum_variant_pattern(const ast::EnumVariantPattern *node);

  void dump_enum_declaration(const ast::EnumDeclaration *node);
  void dump_variable_declaration(const ast::VariableDeclaration *node);
  void dump_function_declaration(const ast::FunctionDeclaration *node);
  void dump_module_declaration(const ast::ModuleDeclaration *node);
  void dump_module_init_declaration(const ast::ModuleInitDeclaration *node);
  void dump_capability_declaration(const ast::CapabilityDeclaration *node);

  // Other nodes
  void dump_named_pattern(const ast::NamedPattern *node);
  void dump_type(const ast::Type *node);
  void dump_object_literal(const ast::ObjectLiteral *node);
  void dump_object_field(const ast::ObjectField *node);
  void dump_array_literal(const ast::ArrayLiteral *node);
  void dump_field_declaration(const ast::FieldDeclaration *node);
  void dump_struct_declaration(const ast::StructDeclaration *node);
  void dump_type_declaration(const ast::TypeDeclaration *node);
  void dump_named_type(const ast::NamedType *node);
  void dump_generic_type(const ast::GenericType *node);
  void dump_name(const ast::NameNode *node);

private:
  AstDispatcher<AstDumper, const celestia::ast::Node> dispatcher;
  DumpContext context;
};

} // namespace celestia::debug
