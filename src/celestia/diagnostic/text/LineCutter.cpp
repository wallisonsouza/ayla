#include "LineCutter.hpp"

#include <cstdint>

#include "celestia/utils/Utf8.hpp"

namespace diagnostic {

const char *LineCutter::compute_start(const char *line_begin, const char *span_begin) {
  

  const char *p = span_begin;

  uint32_t back = 0;

  while (p > line_begin && back < start_context) {
    do { --p; } while (((*p) & 0b11000000) == 0b10000000);

    ++back;
  }

  return p;
}

const char *LineCutter::compute_end(const char *line_end, const char *span_end) {

  const char *p = span_end;

  uint32_t forward = 0;

  while (p < line_end && forward < end_context) {
    auto len = utils::Utf8::length(static_cast<uint8_t>(*p));

    p += len;

    ++forward;
  }

  return p;
}

LineCut LineCutter::cut(const core::source::Source &source, const SourceSlice &slice) {
  auto line = source.buffer.get_line(slice.begin.line);

  const char *line_begin = line.data();
  const char *line_end = line_begin + line.size();

  const char *span_begin = source.buffer.begin() + slice.begin.offset;

  const char *span_end = source.buffer.begin() + slice.end.offset;

  LineCut result;

  result.start = compute_start(line_begin, span_begin);
  result.end = compute_end(line_end, span_end);

  if (result.start != line_begin) {
    result.text += "...";
    result.prefix_offset = 3;
  }

  result.text.append(result.start, result.end);

  if (result.end != line_end) { result.text += "..."; }

  return result;
}

} // namespace diagnostic