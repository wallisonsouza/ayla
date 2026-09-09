#pragma once

#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/semantic/id/ids.hpp"

#include <memory>
#include <vector>

class UnitManager {
public:
  celestia::semantic::CompilationUnitId create(core::source::Source &source) {

    auto id = celestia::semantic::CompilationUnitId(static_cast<uint32_t>(units.size()));

    auto unit = std::make_unique<CompilationUnit>(source);

    unit->id = id;

    units.push_back(std::move(unit));

    return id;
  }

  CompilationUnit *get(celestia::semantic::CompilationUnitId id) {

    if (!id.is_valid()) return nullptr;

    auto index = static_cast<std::size_t>(id.index());

    if (index >= units.size()) return nullptr;

    return units[index].get();
  }

  const CompilationUnit *get(celestia::semantic::CompilationUnitId id) const {

    if (!id.is_valid()) return nullptr;

    auto index = static_cast<std::size_t>(id.index());

    if (index >= units.size()) return nullptr;

    return units[index].get();
  }

  auto &all() { return units; }

  const auto &all() const { return units; }

private:
  std::vector<std::unique_ptr<CompilationUnit>> units;
};