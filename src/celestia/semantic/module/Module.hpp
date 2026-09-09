#pragma once

#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/module/ModuleState.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace celestia::semantic {

class Module {

public:
  // CompilationUnitId unit_;

  Module(std::string name, ModuleId parent = ModuleId::invalid()) : name_(std::move(name)), parent_(parent) {}

  std::string_view name() const { return name_; }

  ScopeId scope_id() const { return scope_; }

  void set_scope(ScopeId scope) { scope_ = scope; }

  ModuleId parent_id() const { return parent_; }

  void add_import(ModuleId module) { imports_.push_back(module); }

  const std::vector<ModuleId> &imports() const { return imports_; }

  ModuleState state() const { return state_; }

  bool has_state(ModuleState state) const { return static_cast<uint8_t>(state_ & state) != 0; }

  void add_state(ModuleState state) { state_ = state_ | state; }

  void remove_state(ModuleState state) { state_ = static_cast<ModuleState>(static_cast<uint8_t>(state_) & ~static_cast<uint8_t>(state)); }

private:
  std::string name_;

  ModuleId parent_ = ModuleId::invalid();

  std::vector<ModuleId> imports_;

  ScopeId scope_ = ScopeId::invalid();

  ModuleState state_ = ModuleState::Discovered;
};

} // namespace celestia::semantic