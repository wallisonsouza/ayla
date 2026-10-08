#pragma once

#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/core/source/Source.hpp"

#include "celestia/diagnostic/Diagnostic.hpp"
#include "celestia/diagnostic/DiagnosticMessages.hpp"
#include "celestia/diagnostic/Formatter.hpp"
#include "celestia/diagnostic/StyledText.hpp"
#include "celestia/diagnostic/Theme.hpp"
#include "celestia/diagnostic/text/LineCutter.hpp"
#include "celestia/diagnostic/text/MarkerBuilder.hpp"

#include "debug/console/color.hpp"
#include "debug/console/console.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

namespace diagnostic {

// -----------------------------------------------------------------------------
// Marker configuration
// -----------------------------------------------------------------------------

inline char marker_symbol(LabelCode code) {
  switch (code) {
  case LabelCode::ExpectedType: return '~';

  case LabelCode::FoundType:
  case LabelCode::LeftOperand:
  case LabelCode::RightOperand: return '^';

  default: return '^';
  }
}

// -----------------------------------------------------------------------------
// Prepared label data
// -----------------------------------------------------------------------------

struct PreparedAnnotation {
  std::size_t column;
  StyledText text;
};

struct PreparedLabels {
  std::string markers;
  std::vector<PreparedAnnotation> annotations;
};

// -----------------------------------------------------------------------------
// Diagnostic header
// -----------------------------------------------------------------------------

inline void print_header(const Diagnostic &diagnostic, const core::source::Source &source, const CompilerEnvironment &env) {

  const auto message = get_message(diagnostic.code);

  const auto text = DiagnosticFormatter::format(message.text, diagnostic, env, source);

  debug::Console::log(debug::Color::MediumSlateBlue, "error", "[", message.origin, "]: ", debug::Color::SoftRed, text);
}

inline void print_file_info(SourceSlice slice, const core::source::Source &source) {

  const auto absolute = std::filesystem::absolute(source.path);

  debug::Console::log(debug::Color::White, " --> ", debug::Color::BrightBlack, "\"", absolute.string(), "\"", theme::Separator, ":", slice.begin.line, ":", slice.begin.column);
}

// -----------------------------------------------------------------------------
// Label preparation
// -----------------------------------------------------------------------------

inline PreparedLabels prepare_labels(const std::vector<Label> &labels, const LineCut &cut, MarkerBuilder &marker, const CompilerEnvironment &env, const core::source::Source &source) {

  PreparedLabels prepared;

  prepared.annotations.reserve(labels.size());

  for (const auto &label : labels) {
    const auto symbol = marker_symbol(label.code);
    const std::string mark(1, symbol);

    const auto underline = marker.underline(cut, label.slice.get_span(), " ", mark);

    const auto column = underline.find_first_not_of(' ');

    // Ignora spans que não aparecem no trecho renderizado.
    if (column == std::string::npos) { continue; }

    // Combina os marcadores em uma única linha.
    if (prepared.markers.size() < underline.size()) { prepared.markers.resize(underline.size(), ' '); }

    for (std::size_t i = 0; i < underline.size(); ++i) {
      if (underline[i] != ' ') { prepared.markers[i] = underline[i]; }
    }

    const auto message_it = label_messages.find(label.code);

    if (message_it == label_messages.end()) { continue; }

    prepared.annotations.push_back({
        .column = column,
        .text = DiagnosticFormatter::format_styled(message_it->second.text, label.code, label.arguments, env, source),
    });
  }

  // Ordena pela posição visual, da esquerda para a direita.
  std::stable_sort(prepared.annotations.begin(), prepared.annotations.end(), [](const PreparedAnnotation &a, const PreparedAnnotation &b) { return a.column < b.column; });

  return prepared;
}

// -----------------------------------------------------------------------------
// Source line
// -----------------------------------------------------------------------------

inline void print_source_line(const LineCut &cut, std::size_t line_number) { debug::Console::log(debug::Color::BrightBlack, line_number, theme::Separator, " | ", theme::Source, cut.text); }

// -----------------------------------------------------------------------------
// Combined markers
// -----------------------------------------------------------------------------

inline void print_labels(const PreparedLabels &labels) {
  if (labels.markers.empty()) { return; }

  debug::Console::log(debug::Color::BrightBlack, " ", theme::Separator, " | ", debug::Color::SoftYellow, labels.markers);
}

// -----------------------------------------------------------------------------
// Annotation colors
// -----------------------------------------------------------------------------

inline debug::Color annotation_color(TextRole role) {
  switch (role) {
  case TextRole::Type: return debug::Color::SoftYellow;
  case TextRole::Symbol: return debug::Color::SoftCyan;
  case TextRole::String: return debug::Color::SoftGreen;
  case TextRole::Number: return debug::Color::SoftMagenta;
  default: return debug::Color::White;
  }
}

// -----------------------------------------------------------------------------
// Hierarchical annotations
// -----------------------------------------------------------------------------

inline void print_labels_hierarchy(const std::vector<PreparedAnnotation> &annotations) {

  for (std::size_t i = annotations.size(); i-- > 0;) {
    const auto &annotation = annotations[i];

    std::string connectors(annotation.column + 1, ' ');

    for (std::size_t j = 0; j <= i; ++j) { connectors[annotations[j].column] = '|'; }

    // Imprime os conectores sem encerrar a linha.
    debug::Console::write(theme::Separator, " ", " | ", debug::Color::SoftYellow, connectors, " ");

    // Imprime cada segmento com sua cor semântica.
    for (const auto &segment : annotation.text) { debug::Console::write(annotation_color(segment.role), segment.text); }

    // Encerra a linha.
    debug::Console::log(debug::Color::White, "");
  }
}

// -----------------------------------------------------------------------------
// Diagnostic entry point
// -----------------------------------------------------------------------------

inline void print_diagnostic(const Diagnostic &diagnostic, const core::source::Source &source, const CompilerEnvironment &env) {

  print_header(diagnostic, source, env);
  print_file_info(diagnostic.primary_slice, source);

  MarkerBuilder marker(source.buffer);
  LineCutter cutter;

  const auto cut = cutter.cut(source, diagnostic.primary_slice);

  const auto labels = prepare_labels(diagnostic.labels, cut, marker, env, source);

  print_source_line(cut, diagnostic.primary_slice.begin.line);
  print_labels(labels);
  print_labels_hierarchy(labels.annotations);
}

} // namespace diagnostic