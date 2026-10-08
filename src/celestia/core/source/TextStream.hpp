#pragma once

#include "SourceBuffer.hpp"
#include "celestia/core/token/Location.hpp"
#include "celestia/utils/Utf8.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace core::source {

struct State {
  const char *ptr;

  uint32_t offset;
  uint32_t line;
  uint32_t column;

  [[nodiscard]]
  SourceSlice slice_to(const State &end) const noexcept {

    return {.begin =
                {
                    .offset = offset,
                    .line = line,
                    .column = column,
                },

            .end = {
                .offset = end.offset,
                .line = end.line,
                .column = end.column,
            }};
  }
};

class TextStream {
public:
private:
  const char *begin;
  const char *current;
  const char *end;

  uint32_t line = 1;
  uint32_t column = 1;

  std::vector<State> checkpoints;

public:
  explicit TextStream(const SourceBuffer &buf) : begin(buf.begin()), current(buf.begin()), end(buf.end()) {}

  bool eof() const noexcept { return current >= end; }

  char32_t peek() const noexcept {
    if (eof()) return U'\0';

    return utils::Utf8::decode(current).codepoint;
  }

  char32_t peek_n(size_t n) const noexcept {
    const char *p = current;

    while (n--) {
      if (p >= end) return U'\0';

      p = utils::Utf8::advance(p);
    }

    if (p >= end) return U'\0';

    return utils::Utf8::decode(p).codepoint;
  }

  char32_t advance() noexcept {
    if (eof()) return U'\0';

    auto result = utils::Utf8::decode(current);

    current += result.size;

    if (result.codepoint == U'\n') {
      line++;
      column = 1;
    } else {
      column++;
    }

    return result.codepoint;
  }

  void advance_n(size_t n) noexcept {
    while (n--) advance();
  }

  SourceSlice slice_from(const State &start) const noexcept { return start.slice_to(get_state()); }

  State get_state() const noexcept {
    return {
        .ptr = current,
        .offset = static_cast<uint32_t>(current - begin),
        .line = line,
        .column = column,
    };
  }

  void rollback(const State &state) noexcept {
    current = state.ptr;
    line = state.line;
    column = state.column;
  }

  void push_checkpoint() { checkpoints.push_back(get_state()); }

  void rollback_checkpoint() {
    if (!checkpoints.empty()) {
      rollback(checkpoints.back());
      checkpoints.pop_back();
    }
  }

  void discard_checkpoint() {
    if (!checkpoints.empty()) checkpoints.pop_back();
  }
};

} // namespace core::source