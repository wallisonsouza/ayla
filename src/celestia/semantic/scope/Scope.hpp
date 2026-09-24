#pragma once

#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/symbol/SymbolTable.hpp"

#include <string_view>

namespace core {

enum class ScopeKind {
    Module,
    Struct,
    Enum,
    Type,
    Capability,
    Impl,
    Function,
    Block
};

struct Scope {

  SymbolTable symbols;
  ScopeKind kind;

  celestia::semantic::ScopeId id;
  celestia::semantic::ScopeId parent;

  explicit Scope(ScopeKind kind, celestia::semantic::ScopeId parent = celestia::semantic::ScopeId::invalid()) : kind(kind), parent(parent) {}

  bool has_symbol_local(std::string_view name) const { return symbols.contains(name); }
};

} // namespace core