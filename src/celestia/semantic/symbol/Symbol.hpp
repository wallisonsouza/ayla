#pragma once

#include "SymbolKind.hpp"
#include "celestia/core/modifiers/ModifierSet.hpp"
#include "celestia/core/modifiers/Visibility.hpp"
#include "celestia/semantic/id/ids.hpp"
#include "celestia/ast/ASTFwd.hpp"
#include <string>
struct Symbol {

  celestia::semantic::SymbolId id;
  celestia::semantic::ModuleId module;
  celestia::semantic::TypeId type;
  SymbolKind kind;

  std::string name;

  Visibility visibility = Visibility::Private;

  ModifierSet modifiers;

  celestia::ast::Node *declaration = nullptr;

  Symbol(celestia::semantic::SymbolId id, std::string name, SymbolKind kind, Visibility visibility, bool builtin, celestia::ast::Node *decl)
      : id(id), name(std::move(name)), kind(kind), visibility(visibility), declaration(decl) {}
};