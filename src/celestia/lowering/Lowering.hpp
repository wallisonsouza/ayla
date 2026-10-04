#pragma once

#include "celestia/ast/AstDispacher.hpp"

#include "celestia/ast/declaration/ImplementationDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"

#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/IdentifierExpression.hpp"

#include "celestia/lowering/LoweringContext.hpp"

namespace celestia::lowering {

class Lowering {
public:
  Lowering(LoweringContext &context) : context(context) {}

  void lower() {
    // std::cout << "[Lowering] lowering unit " << context.unit.id.index() << " module '" << context.unit.root->name->get_str() << "'\n";

    // lower_module_declaration(context.unit.root);
  }

private:
  LoweringContext &context;

  void lower_variable_declaration(const ast::VariableDeclaration *node);

  void lower_struct_declaration(const ast::StructDeclaration *node);

  void lower_capability_declaration(const ast::CapabilityDeclaration *node);

  void lower_impl_declaration(const ast::ImplDeclaration *node);

  void lower_function_declaration(const ast::FunctionDeclaration *node);

  void lower_declaration(const ast::Declaration *node);

  void lower_module_declaration(const ast::ModuleDeclaration *node);

  ir::ValueId lower_expression(const ast::Expression *node);

  ir::ValueId lower_number_literal(const ast::NumberLiteral *node);

  ir::ValueId lower_string_literal(const ast::StringLiteral *node);

  ir::ValueId lower_identifier(const ast::IdentifierExpressionNode *node);

  ir::ValueId lower_bool_literal(const ast::BoolLiteral *node);

  ir::ValueId lower_binary_expression(const ast::BinaryExpressionNode *node);

  void lower_pattern(const ast::PatternNode *node, ir::ValueId value);

  ir::TypeId lower_type(semantic::TypeId type_id);

  AstDispatcher<Lowering, const ast::Expression> dispatcher;
};

} // namespace celestia::lowering