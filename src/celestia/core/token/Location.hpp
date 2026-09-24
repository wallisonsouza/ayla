#pragma once

#include "celestia/core/source/Span.hpp"
#include <cstdint>

struct Location {
  uint32_t offset{};
  uint32_t line{};
  uint32_t column{};
};

struct SourceSlice {
  Location begin;
  Location end;

  [[nodiscard]]
  core::source::Span get_span() const noexcept {
    return {
        begin.offset,
        end.offset,
    };
  }

  void extend_to(SourceSlice other) { end = other.end; }
};