#include "celestia/ast/expression/ArrayLiteral.hpp"
#include "celestia/ast/expression/ObjectLiteral.hpp"
#include "celestia/ast/expression/StructLiteral.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
#include <iostream>

namespace celestia::semantic {

// void Resolver::number_literal(celestia::ast::NumberLiteral *node) {}

// void Resolver::string_literal(celestia::ast::StringLiteral *node) {}

// void Resolver::boolean_literal(celestia::ast::BoolLiteral *node) {}

void Resolver::object_literal(celestia::ast::ObjectLiteral *node) {

  if (!node) return;

  for (auto *field : node->fields) {
    if (field && field->value) resolve_node(field->value);
  }
}

void Resolver::array_literal(celestia::ast::ArrayLiteral *node) {

  if (!node) return;

  for (auto *element : node->elements) {
    if (element) resolve_node(element);
  }
}

void Resolver::struct_literal(ast::StructLiteral *node) {

  if (!node) return;

  if (!node->type) {
    std::cerr << "[Resolver] struct literal has no type\n";
    return;
  }

  resolve_node(node->type);

  auto type = context.unit.semantic.type(node->type);

  if (!type.is_valid()) {
    std::cerr << "[Resolver] struct literal type has no symbol\n";
    return;
  }

  auto symbol = context.unit.semantic.symbol(node->type);

  context.unit.semantic.set_symbol(node, symbol);

  for (auto *field : node->fields) {

    if (!field) continue;

    if (field->value) resolve_node(field->value);
  }
}
} // namespace celestia::semantic