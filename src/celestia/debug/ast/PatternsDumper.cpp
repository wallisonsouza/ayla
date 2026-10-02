#include "celestia/debug/ast/AstDumper.hpp"
#include <format>

namespace celestia::debug {
void AstDumper::dump_named_pattern(const ast::NamedPattern *node) {

  auto g = context.object(std::format("NamedPattern(\"{}\")", node->name->str));

  g.field("TypeAnnotation", node->type_annotation);
}

void AstDumper::dump_enum_variant_pattern(const ast::EnumVariantPattern *node) {

  auto g = context.object(std::format("EnumVariantPattern"));

  g.field("name", node->name);
  g.list("arguments", node->arguments);
}

} // namespace celestia::debug