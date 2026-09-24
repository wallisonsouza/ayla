

#include "celestia/debug/ast/AstDumper.hpp"
#include <format>

namespace celestia::debug {

void AstDumper::dump_identifier(const ast::Identifier *node) {

  //
  auto g = context.object(std::format("Identifier(\"{}\")", node->get_str()));
}

void AstDumper::dump_qualified_name(const ast::QualifiedName *node) {
  
  auto g = context.object("QualifiedName");

  g.list("Parts", node->parts);
}

void AstDumper::dump_generic_name(const ast::GenericName *node) {
  
  auto g = context.object("GenericName");

  g.field("name", node->name);

  g.list("generics", node->parameters);
}

} // namespace celestia::debug
