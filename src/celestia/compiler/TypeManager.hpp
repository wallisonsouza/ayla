#pragma once

#include "celestia/core/memory/Arena.hpp"
#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/types/type.hpp"

#include <unordered_map>
#include <vector>

class TypeManager {

public:
  celestia::semantic::TypeId get_or_create(celestia::semantic::SymbolId symbol, celestia::semantic::TypeKind kind) {

    auto it = types_by_symbol.find(symbol);

    if (it != types_by_symbol.end()) return it->second;

    celestia::semantic::Type *type = nullptr;

    switch (kind) {

    case celestia::semantic::TypeKind::Struct: type = arena.alloc<celestia::semantic::StructType>(symbol); break;

    case celestia::semantic::TypeKind::Function: type = arena.alloc<celestia::semantic::FunctionType>(); break;

    default: return celestia::semantic::TypeId::invalid();
    }

    auto id = insert(type);

    types_by_symbol.emplace(symbol, id);

    return id;
  }

  celestia::semantic::TypeId get_or_create_primitive(celestia::semantic::SymbolId symbol, celestia::semantic::PrimitiveKind primitive) {

    auto it = types_by_symbol.find(symbol);

    if (it != types_by_symbol.end()) return it->second;

    auto *type = arena.alloc<celestia::semantic::PrimitiveType>(primitive);

    auto id = insert(type);

    types_by_symbol.emplace(symbol, id);

    return id;
  }

  celestia::semantic::TypeId get_or_create_generic(celestia::semantic::SymbolId symbol, size_t arity) {

    auto it = types_by_symbol.find(symbol);

    if (it != types_by_symbol.end()) return it->second;

    auto *type = arena.alloc<celestia::semantic::GenericDeclarationType>(symbol, arity);

    auto id = insert(type);

    types_by_symbol.emplace(symbol, id);

    return id;
  }

  celestia::semantic::TypeId get_or_create_generic_instance(celestia::semantic::TypeId constructor, const std::vector<celestia::semantic::TypeId> &arguments) {

    for (size_t i = 0; i < types.size(); ++i) {

      const auto &base = *types[i];

      if (base.kind != celestia::semantic::TypeKind::GenericInstance) continue;

      const auto &generic = static_cast<const celestia::semantic::GenericInstanceType &>(base);

      if (generic.constructor != constructor) continue;

      if (generic.arguments != arguments) continue;

      return celestia::semantic::TypeId(static_cast<celestia::semantic::TypeId::ValueType>(i));
    }

    auto *type = arena.alloc<celestia::semantic::GenericInstanceType>(constructor, arguments);

    return insert(type);
  }

  celestia::semantic::Type &get(celestia::semantic::TypeId id) { return *types[id.index()]; }

  const celestia::semantic::Type &get(celestia::semantic::TypeId id) const { return *types[id.index()]; }

private:
  celestia::semantic::TypeId insert(celestia::semantic::Type *type) {

    celestia::semantic::TypeId id(static_cast<celestia::semantic::TypeId::ValueType>(types.size()));

    types.push_back(type);

    return id;
  }

private:
  core::memory::Arena arena;

  std::vector<celestia::semantic::Type *> types;

  std::unordered_map<celestia::semantic::SymbolId, celestia::semantic::TypeId> types_by_symbol;
};