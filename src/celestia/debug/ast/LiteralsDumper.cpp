
#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/debug/ast/AstDumper.hpp"
#include <format>

namespace celestia::debug {

void AstDumper::dump_number_literal(const ast::NumberLiteralNode *node) {
  auto g = context.object(std::format("Number({})", node->value));

  (void)g;
}

void AstDumper::dump_string_literal(const ast::StringLiteralNode *node) {
  auto g = context.object(std::format("String(\"{}\")", node->value));

  (void)g;
}

void AstDumper::dump_bool_literal(const ast::BoolLiteralNode *node) {
  auto g = context.object(std::format("Boolean({})", node->value ? "true" : "false"));

  (void)g;
}

void AstDumper::dump_null_literal(const ast::NullLiteralNode *node) {
  (void)node;

  auto g = context.object("Null");

  (void)g;
}

void AstDumper::dump_struct_field(const ast::StructField *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.field("Name", node->name);
  g.field("Value", node->value);
}

void AstDumper::dump_struct_literal(const ast::StructLiteral *node) {

  auto g = context.object(ast::node_kind_name(node->kind));

  g.list("Fields", node->fields);
  
}
void AstDumper::dump_array_literal(const ast::ArrayLiteralNode *node) {
  auto g = context.object("ArrayLiteral");

  g.list("Elements", node->elements);
}

void AstDumper::dump_object_literal(const ast::ObjectLiteral *node) {
  auto g = context.object("ObjectLiteral");

  g.list("Fields", node->fields);
}
} // namespace celestia::debug