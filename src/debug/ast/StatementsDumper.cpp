#include "debug/ast/AstDumper.hpp"



namespace celestia::debug {


void AstDumper::dump_return_statement(const ast::ReturnStatement *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Value", node->value);
}

void AstDumper::dump_expression_statement(const ast::ExpressionStatement *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Expression", node->expression);
}

void AstDumper::dump_import_statement(const ast::ImportDeclaration *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("name", node->name);

  if (node->path.has_value()) { g.field("path", node->path.value()); }

  // if (node->name) g.field("Path",node->name->get_str());
}
} // namespace celestia::debug