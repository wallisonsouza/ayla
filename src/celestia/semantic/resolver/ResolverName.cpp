#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

void Resolver::identifier(ast::IdentifierExpressionNode *node) {

  if (!node || !node->name) return;

  ScopeId scope_id = context.unit.semantic.scope(node);

  if (!scope_id.is_valid()) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::UndefinedSymbol,
        .arguments =
            {
                diagnostic::name(node->name->str),
            },
    });

    return;
  }

  SymbolId id = context.get_env().scopes.lookup(scope_id, node->name->str);

  if (!id.is_valid()) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::UndefinedSymbol,
        .arguments =
            {
                diagnostic::name(node->name->str),
            },
    });

    return;
  }

  context.unit.semantic.set_symbol(node, id);
}

SymbolId Resolver::resolve_name(ast::NameNode *name, ScopeId scope_id) {

  assert(name && "Resolver::resolve_name received null");

  switch (name->kind) {

  case ast::NodeKind::Identifier: return lookup_symbol(scope_id, static_cast<ast::IdentifierNode *>(name)->str);

  case ast::NodeKind::QualifiedName: return lookup_qualified_name(static_cast<ast::QualifiedNameNode *>(name));

  default: assert(false && "Unsupported NameNode kind"); return SymbolId::invalid();
  }
}

SymbolId Resolver::lookup_symbol(ScopeId scope_id, std::string_view name) const { return context.get_env().scopes.lookup(scope_id, name); }

SymbolId Resolver::lookup_qualified_name(ast::QualifiedNameNode *name) {

  assert(name && "lookup_qualified_name received null");

  const auto &parts = name->parts;

  if (parts.empty()) { return SymbolId::invalid(); }

  if (parts.size() == 1) { return SymbolId::invalid(); }

  std::string module_name;

  for (std::size_t i = 0; i + 1 < parts.size(); ++i) {

    if (!module_name.empty()) { module_name += '.'; }

    module_name += parts[i]->str;
  }

  ModuleId module_id = context.get_env().modules.find(module_name);

  if (!module_id.is_valid()) { return SymbolId::invalid(); }

  const auto &module = context.get_env().modules.get(module_id);

  return context.get_env().scopes.lookup(module.scope_id(), parts.back()->str);
}

} // namespace celestia::semantic