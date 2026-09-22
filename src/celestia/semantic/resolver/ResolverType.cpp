#include "celestia/semantic/resolver/Resolver.hpp"

#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

#include <cassert>

namespace celestia::semantic {

void Resolver::type_node(ast::TypeNode *node) {

  assert(node && "Resolver::type_node received null");

  debug::Trace::log(debug::Category::Resolver, "resolving type node '{}'", ast::node_kind_name(node->kind));

  switch (node->kind) {

  case ast::NodeKind::NamedType: resolve_named_type(static_cast<ast::NamedType *>(node)); return;

  case ast::NodeKind::GenericType: resolve_generic_type(static_cast<ast::GenericTypeNode *>(node)); return;

  case ast::NodeKind::FunctionType: resolve_function_type(static_cast<ast::FunctionType *>(node)); return;

  default: assert(false && "Resolver::type_node received unsupported NodeKind"); return;
  }
}

void Resolver::resolve_named_type(ast::NamedType *node) {

  assert(node && node->name);

  ScopeId scope_id = context.stack.current();

  if (!scope_id.is_valid()) { return; }

  SymbolId symbol_id = resolve_name(node->name, scope_id);

  if (!symbol_id.is_valid()) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::UndefinedSymbol,
        .arguments =
            {
                diagnostic::name(node->name->get_str()),
            },
        .labels =
            {
                diagnostic::location(node->slice),
            },
    });

    return;
  }

  context.unit.semantic.set_symbol(node, symbol_id);
}
void Resolver::resolve_generic_type(ast::GenericTypeNode *node) {

  assert(node);
  assert(node->name);

  std::cout << "no generic" << std::endl;

  // Resolve Add
  resolve_node(node->name);

  // Resolve B e F
  for (auto *argument : node->arguments) {
    assert(argument);
    type_node(argument);
  }
}
void Resolver::resolve_function_type(ast::FunctionType *node) {

  // assert(node && "Resolver::resolve_function_type received null");

  // debug::Trace::log(debug::Category::Resolver, "resolving function type with {} parameters", node->parameters.size());

  // for (auto *parameter : node->parameters) {

  //   assert(parameter && "FunctionType contains null parameter");

  //   type_node(parameter);
  // }

  // if (node->return_type) {

  //   debug::Trace::log(debug::Category::Resolver, "resolving function return type");

  //   type_node(node->return_type);
  // }

  // debug::Trace::log(debug::Category::Resolver, "function type resolved");
}

} // namespace celestia::semantic
