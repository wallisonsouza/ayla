#include "celestia/semantic/module/ModuleLookup.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

SymbolId Resolver::require_symbol(ast::NameNode *name, ScopeId scope_id) {
  assert(name);

  auto symbol = NameLookup::lookup(context.get_env(), name, scope_id);

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