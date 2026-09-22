#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/debug/ast/AstDumper.hpp"

#include <format>

namespace celestia::debug {

void AstDumper::dump_generic(const ast::GenericParameter *node) {

  auto g = context.object("Generic");

  g.field("name", node->name);

  g.list("constraints", node->constraints);
}

void AstDumper::dump_name(const ast::NameNode *node) {

  if (!node) return;

  switch (node->kind) {

  case ast::NodeKind::Identifier: dump_identifier(static_cast<const ast::IdentifierNode *>(node)); break;

  case ast::NodeKind::QualifiedName: dump_qualified_name(static_cast<const ast::QualifiedNameNode *>(node)); break;

  default: break;
  }
}

void AstDumper::dump_type(const ast::TypeNode *node) {

  if (!node) return;

  switch (node->kind) {

  case celestia::ast::NodeKind::NamedType: dump_named_type(static_cast<const ast::NamedType *>(node)); break;

  default: break;
  }
}

void AstDumper::dump_named_type(const ast::NamedType *node) {

  if (!node || !node->name) return;

  auto g = context.object("NamedType");

  g.field("name", node->name);
}

void AstDumper::dump_generic_type(const ast::GenericTypeNode *node) {

  if (!node || !node->name) return;

  auto g = context.object("GenericType");


  g.field("name", node->name);

  g.list("Arguments", node->arguments);
}

void AstDumper::dump_object_field(const ast::ObjectFieldNode *node) {

  if (!node) return;

  auto g = context.object("Field");

  g.field("Key", node->key);
  g.field("Value", node->value);
}

void AstDumper::dump_root(const ast::RootNode *node) {

  if (!node) return;

  auto g = context.object("Root");

  g.list("modules", node->modules);
}

} // namespace celestia::debug