#pragma once
#include <cstdint>

enum class CompileFlags : uint32_t {
  None = 0,
  Lexed = 1u << 0,
  Parsed = 1u << 1,
  Resolved = 1u << 2,
  Checked = 1u << 3,
  Lowered = 1u << 4,
};

