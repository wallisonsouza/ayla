#include "celestia/core/memory/Arena.hpp"
#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/scope/Scope.hpp"

#include <cassert>
#include <vector>

class ScopeManager {
private:
  core::memory::Arena arena;
  std::vector<core::Scope *> scopes;

public:
  celestia::semantic::ScopeId create_scope(core::ScopeKind kind, celestia::semantic::ScopeId parent = celestia::semantic::ScopeId::invalid()) {

    auto *scope = arena.alloc<core::Scope>(kind, parent);

    celestia::semantic::ScopeId id{static_cast<uint32_t>(scopes.size())};

    assert(id.is_valid());

    scope->id = id;
    scopes.push_back(scope);

    return id;
  }

  core::Scope &get(celestia::semantic::ScopeId id) { return *scopes.at(id.index()); }

  const core::Scope &get(celestia::semantic::ScopeId id) const { return *scopes.at(id.index()); }

  celestia::semantic::SymbolId lookup(celestia::semantic::ScopeId scope_id, std::string_view name) const {

    while (scope_id.is_valid()) {

      const auto &scope = get(scope_id);

      celestia::semantic::SymbolId symbol = scope.symbols.find(name);

      if (symbol.is_valid()) return symbol;

      scope_id = scope.parent;
    }

    return celestia::semantic::SymbolId::invalid();
  }
};