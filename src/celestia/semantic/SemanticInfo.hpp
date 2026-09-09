#pragma once

#include "celestia/ast/Node.hpp"
#include "celestia/semantic/id/ids.hpp"

#include <unordered_map>

namespace celestia::semantic {

class SemanticInfo {
private:
  std::unordered_map<const ast::Node *, SymbolId> symbols;
  std::unordered_map<const ast::Node *, TypeId> types;
  std::unordered_map<const ast::Node *, ScopeId> scopes;
  std::unordered_map<const ast::Node *, ModuleId> modules;

public:
  // Symbol
  void set_symbol(ast::Node *node, SymbolId symbol) { symbols[node] = symbol; }

  SymbolId symbol(const ast::Node *node) const {
    auto it = symbols.find(node);

    if (it == symbols.end()) return SymbolId::invalid();

    return it->second;
  }

  // Type
  void set_type(ast::Node *node, TypeId type) { types[node] = type; }

  TypeId type(const ast::Node *node) const {
    auto it = types.find(node);

    if (it == types.end()) return TypeId::invalid();

    return it->second;
  }

  // Scope
  void set_scope(ast::Node *node, ScopeId scope) { scopes[node] = scope; }

  ScopeId scope(const ast::Node *node) const {
    auto it = scopes.find(node);

    if (it == scopes.end()) return ScopeId::invalid();

    return it->second;
  }

  // Module
  void set_module(ast::Node *node, ModuleId module) { modules[node] = module; }

  ModuleId module(const ast::Node *node) const {
    auto it = modules.find(node);

    if (it == modules.end()) return ModuleId::invalid();

    return it->second;
  }
};

} // namespace celestia::semantic
