#pragma once

#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/module/Module.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace celestia::semantic {

class ModuleManager {
public:
  ModuleId register_module(std::string_view name, ModuleId parent = ModuleId::invalid()) {

    auto it = modules_.find(std::string(name));

    if (it != modules_.end()) { return it->second; }

    ModuleId id(static_cast<uint32_t>(storage_.size()));

    auto module = std::make_unique<Module>(std::string(name), parent);

    storage_.push_back(std::move(module));
    modules_.emplace(std::string(name), id);

    return id;
  }

  ModuleId find(std::string_view name) const {

    auto it = modules_.find(std::string(name));

    if (it == modules_.end()) { return ModuleId::invalid(); }

    return it->second;
  }

  void add_provider(ModuleId module, CompilationUnitId unit) {

    if (!module.is_valid() || !unit.is_valid()) { return; }

    auto &units = providers_[module];

    for (auto existing : units) {
      if (existing == unit) { return; }
    }

    units.push_back(unit);
  }

  const std::vector<CompilationUnitId> &providers(ModuleId module) const {

    static const std::vector<CompilationUnitId> empty;

    if (!module.is_valid()) { return empty; }

    auto it = providers_.find(module);

    if (it == providers_.end()) { return empty; }

    return it->second;
  }

  Module &get(ModuleId id) {

    if (!id.is_valid()) { throw std::runtime_error("ModuleManager: invalid module id"); }

    auto index = static_cast<std::size_t>(id.index());

    if (index >= storage_.size()) { throw std::runtime_error("ModuleManager: invalid module id"); }

    return *storage_[index];
  }

  const Module &get(ModuleId id) const {

    if (!id.is_valid()) { throw std::runtime_error("ModuleManager: invalid module id"); }

    auto index = static_cast<std::size_t>(id.index());

    if (index >= storage_.size()) { throw std::runtime_error("ModuleManager: invalid module id"); }

    return *storage_[index];
  }

private:

  std::vector<std::unique_ptr<Module>> storage_;
  std::unordered_map<std::string, ModuleId> modules_;

  // ModuleId -> CompilationUnitId[]
  std::unordered_map<ModuleId, std::vector<CompilationUnitId>> providers_;
};

} // namespace celestia::semantic