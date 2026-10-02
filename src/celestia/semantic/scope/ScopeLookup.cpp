#include "celestia/semantic/scope/ScopeLookup.hpp"

namespace celestia::semantic {

SymbolId ScopeLookup::find_local(const CompilerEnvironment &env, ScopeId scope_id, std::string_view name) {
    
  assert(scope_id.is_valid());

  const auto &scope = env.scopes.get(scope_id);

  return scope.symbols.find(name);
}

SymbolId ScopeLookup::lookup(const CompilerEnvironment &env, ScopeId scope_id, std::string_view name) {

  while (scope_id.is_valid()) {

    const auto &scope = env.scopes.get(scope_id);

    auto symbol = scope.symbols.find(name);

    if (symbol.is_valid()) return symbol;

    scope_id = scope.parent;
  }

  return SymbolId::invalid();
}

} // namespace celestia::semantic