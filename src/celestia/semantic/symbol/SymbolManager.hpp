#pragma once

#include "Symbol.hpp"
#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

struct SymbolManager {
private:
  uint32_t next_id = 0;
  std::vector<Symbol> symbols;

public:
  celestia::semantic::SymbolId create_symbol(const std::string &name, SymbolKind kind, Visibility visibility = Visibility::Private, bool is_builtin = false, celestia::ast::Node *decl = nullptr) {

    if (next_id == celestia::semantic::SymbolId::INVALID) { throw std::runtime_error("celestia::semantic::SymbolId overflow"); }

    celestia::semantic::SymbolId id(next_id++);

    symbols.emplace_back(id, name, kind, visibility, is_builtin, decl);
   

    return id;
  }

  Symbol &get(celestia::semantic::SymbolId id) {
    if (!id.is_valid()) { throw std::runtime_error("invalid SymbolId"); }

    const auto index = id.index();

    if (index >= symbols.size()) { throw std::runtime_error("invalid SymbolId"); }

    return symbols[index];
  }

  const Symbol &get(celestia::semantic::SymbolId id) const {
    if (!id.is_valid()) { throw std::runtime_error("invalid SymbolId"); }

    const auto index = id.index();

    if (index >= symbols.size()) { throw std::runtime_error("invalid SymbolId"); }

    return symbols[index];
  }

  size_t size() const { return symbols.size(); }
};