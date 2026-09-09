#pragma once

#include <cstdint>

enum class ModuleState : uint8_t {
  None = 0,
  Discovered = 1 << 0,
  Parsed = 1 << 1,
  Resolved = 1 << 2,
  Checked = 1 << 3,
  Lowered = 1 << 4,
  Failed = 1 << 5,
};

constexpr ModuleState operator|(ModuleState a, ModuleState b) { return static_cast<ModuleState>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b)); }

constexpr ModuleState operator&(ModuleState a, ModuleState b) { return static_cast<ModuleState>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b)); }

constexpr ModuleState operator~(ModuleState a) { return static_cast<ModuleState>(~static_cast<uint8_t>(a)); }