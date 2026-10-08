
#pragma once

#include <string>
#include <vector>

namespace diagnostic {

enum class TextRole { Normal, Type, Symbol, String, Number };

struct TextSegment {
  std::string text;
  TextRole role = TextRole::Normal;
};

using StyledText = std::vector<TextSegment>;

inline void append(StyledText &text, std::string value, TextRole role) {
  if (value.empty()) { return; }

  if (!text.empty() && text.back().role == role) {
    text.back().text += value;
    return;
  }

  text.push_back({
      .text = std::move(value),
      .role = role,
  });
}

inline void replace_styled(StyledText &text, std::string_view from, std::string_view to, TextRole role = TextRole::Normal) {
  if (from.empty()) { return; }

  StyledText result;

  for (const auto &segment : text) {
    std::size_t begin = 0;

    while (true) {
      const auto pos = segment.text.find(from, begin);

      if (pos == std::string::npos) {
        append(result, segment.text.substr(begin), segment.role);
        break;
      }

      append(result, segment.text.substr(begin, pos - begin), segment.role);
      append(result, std::string(to), role);

      begin = pos + from.size();
    }
  }

  text = std::move(result);
}

} // namespace diagnostic