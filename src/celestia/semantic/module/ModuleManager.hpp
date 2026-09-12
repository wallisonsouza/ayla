#pragma once

#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/module/Module.hpp"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace celestia::semantic {

using ModuleProvider = std::filesystem::path;

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
};

} // namespace celestia::semantic