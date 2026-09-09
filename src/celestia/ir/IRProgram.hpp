#pragma once

#include <memory>
#include <vector>

#include "celestia/ir/IRGlobal.hpp"
#include "celestia/ir/Value.hpp"
#include "celestia/semantic/id/ids.hpp"

namespace celestia::ir {

class IRProgram {
private:
  IRGlobalContext global;
  std::vector<std::unique_ptr<IRModule>> modules;

public:
  std::unordered_map<semantic::TypeId, ir::TypeId> type_map;

  ModuleId create_module(StringId name) {

    ModuleId id{static_cast<uint32_t>(modules.size())};

    auto module = std::make_unique<IRModule>();

    module->id = id;
    module->name = name;

    modules.push_back(std::move(module));

    return id;
  }

  IRGlobalContext &get_global() { return global; }

  const IRGlobalContext &get_global() const { return global; }

  IRModule &get_module(ModuleId id) { return *modules[id.index()]; }

  const IRModule &get_module(ModuleId id) const { return *modules[id.index()]; }

  const std::vector<std::unique_ptr<IRModule>> &get_modules() const { return modules; }

  size_t module_count() const { return modules.size(); }
};

} // namespace celestia::ir