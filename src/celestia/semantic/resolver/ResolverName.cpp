#include "celestia/ast/NodeCast.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
namespace celestia::semantic {

SymbolId Resolver::resolve_name(ast::NameNode *name, ScopeId scope_id) {

  assert(name && "Resolver::resolve_name received null");

  switch (name->kind) {

  case ast::NodeKind::Identifier: return context.get_env().scopes.lookup(scope_id, ast::as<ast::Identifier>(name)->get_str());

  case ast::NodeKind::QualifiedName: return lookup_qualified_name(ast::as<ast::QualifiedName>(name));

  default: assert(false && "Unsupported NameNode kind"); return SymbolId::invalid();
  }
}

SymbolId Resolver::lookup_qualified_name(ast::QualifiedName *name) {

  assert(name && "lookup_qualified_name received null");

  const auto &parts = name->parts;

  if (parts.size() < 2) return SymbolId::invalid();

  std::string module_name;

  for (std::size_t i = 0; i + 1 < parts.size(); ++i) {

    if (!module_name.empty()) module_name += '.';

    module_name += parts[i]->str;
  }

  ModuleId module_id = context.get_env().modules.find(module_name);

  if (!module_id.is_valid()) return SymbolId::invalid();

  const auto &module = context.get_env().modules.get(module_id);

  return context.get_env().scopes.lookup(module.scope_id(), parts.back()->str);
}

SymbolId Resolver::require_symbol(ast::NameNode *name, ScopeId scope_id) {

  auto symbol = resolve_name(name, scope_id);

  if (symbol.is_valid()) return symbol;

  context.unit.diagnostics.report({
      .severity = diagnostic::Severity::Error,
      .code = diagnostic::DiagnosticCode::UndefinedSymbol,
      .arguments =
          {
              diagnostic::name(name->get_str()),
          },
      .labels =
          {
              diagnostic::location(name->slice),
          },
  });

  return SymbolId::invalid();
}

} // namespace celestia::semantic