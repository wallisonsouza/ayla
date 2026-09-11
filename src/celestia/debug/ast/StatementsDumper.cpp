#include "celestia/debug/ast/AstDumper.hpp"

namespace celestia::debug {
void AstDumper::dump_while_statement(const ast::WhileStatement *node) {

  auto g = context.object("WhileStatement");

  g.field("Condition", node->condition);
  g.field("Body", node->body);
}

void AstDumper::dump_block_statement(const ast::BlockStatement *node) {

  auto g = context.object("BlockStatement");

  g.list("Statements", node->statements);
}

void AstDumper::dump_if_statement(const ast::IfStatement *node) {

  auto g = context.object("IfStatement");

  g.field("Condition", node->condition);
  g.field("Then", node->then_block);
  g.field("Else", node->else_block);
}

void AstDumper::dump_return_statement(const ast::ReturnStatement *node) {

  auto g = context.object("ReturnStatement");

  g.field("Value", node->value);
}

void AstDumper::dump_expression_statement(const ast::ExpressionStatement *node) {

  auto g = context.object("ExpressionStatement");

  g.field("Expression", node->expression);
}

void AstDumper::dump_import_statement(const ast::ImportDeclaration *node) {

  auto g = context.object("ImportDeclaration");

  g.field("name",node->name);

  if (node->path.has_value()) { g.field("path", node->path.value()); }

  // if (node->name) g.field("Path",node->name->get_str());
}
} // namespace celestia::debug