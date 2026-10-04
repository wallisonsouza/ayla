#pragma once

#include "celestia/ast/Node.hpp"
#include "celestia/core/token/Location.hpp"
#include "celestia/core/token/Token.hpp"
#include "celestia/core/token/TokenKind.hpp"
#include "celestia/diagnostic/DiagnosticCode.hpp"
#include "celestia/diagnostic/Expected.hpp"
#include "celestia/semantic/id/ids.hpp"

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace diagnostic {

//--------------------------------------------------
// Expected
//--------------------------------------------------

enum class ExpectedPosition {
  Before,
  After,
};

struct ExpectedToken {
  TokenKind kind;
  ExpectedPosition position;
};

struct ExpectedCategory {
  ExpectedKind kind;
  ExpectedPosition position;
};

//--------------------------------------------------
// Argument
//--------------------------------------------------

enum class DiagnosticArgumentKind {
  Expected,
  Found,
  Previous,
  Type,
  Symbol,
  SymbolKind,
  Name,
};

using DiagnosticValue = std::variant<ExpectedToken, ExpectedCategory, Token *, TokenKind, celestia::semantic::TypeId, celestia::semantic::SymbolId,  std::string>;

struct DiagnosticArgument {
  DiagnosticArgumentKind kind;
  DiagnosticValue value;
};

//--------------------------------------------------
// Label
//--------------------------------------------------

enum class LabelKind {
  Primary,
  Secondary,
};

struct Label {
  SourceSlice slice;
  LabelKind kind;
};

//--------------------------------------------------
// Severity
//--------------------------------------------------

enum class Severity {
  Error,
  Warning,
  Note,
  Help,
};

//--------------------------------------------------
// Auxiliary diagnostics
//--------------------------------------------------

struct Help {
  HelpCode code;
  std::vector<DiagnosticArgument> arguments;
};

struct Note {
  NoteCode code;
  std::vector<DiagnosticArgument> arguments;
};

//--------------------------------------------------
// Diagnostic
//--------------------------------------------------

struct Diagnostic {
  Severity severity;
  DiagnosticCode code;

  std::vector<DiagnosticArgument> arguments;

  std::vector<Label> labels;
  std::vector<Help> helps;
  std::vector<Note> notes;
};

//--------------------------------------------------
// Label helpers
//--------------------------------------------------

inline Label location(SourceSlice slice, LabelKind kind = LabelKind::Primary) {

  return {
      .slice = slice,
      .kind = kind,
  };
}

inline Label location(const celestia::ast::Node *node, LabelKind kind = LabelKind::Primary) { return location(node->slice, kind); }
//--------------------------------------------------
// Argument helpers
//--------------------------------------------------

template <typename T> inline DiagnosticArgument make_argument(DiagnosticArgumentKind kind, T &&value) {

  return {
      .kind = kind,
      .value = std::forward<T>(value),
  };
}

//--------------------------------------------------
// Expected helpers
//--------------------------------------------------

inline DiagnosticArgument expected_token(TokenKind kind, ExpectedPosition position = ExpectedPosition::Before) {

  return make_argument(DiagnosticArgumentKind::Expected, ExpectedToken{
                                                             .kind = kind,
                                                             .position = position,
                                                         });
}

inline DiagnosticArgument expected_category(ExpectedKind kind, ExpectedPosition position = ExpectedPosition::Before) {

  return make_argument(DiagnosticArgumentKind::Expected, ExpectedCategory{
                                                             .kind = kind,
                                                             .position = position,
                                                         });
}

//--------------------------------------------------
// Other argument helpers
//--------------------------------------------------

inline DiagnosticArgument name(std::string value) { return make_argument(DiagnosticArgumentKind::Name, std::move(value)); }

inline DiagnosticArgument symbol(celestia::semantic::SymbolId value) { return make_argument(DiagnosticArgumentKind::Symbol, value); }

inline DiagnosticArgument type(celestia::semantic::TypeId value) { return make_argument(DiagnosticArgumentKind::Type, value); }

inline DiagnosticArgument found_type(celestia::semantic::TypeId value) { return make_argument(DiagnosticArgumentKind::Found, value); }

inline DiagnosticArgument found(Token *value) { return make_argument(DiagnosticArgumentKind::Found, value); }

inline DiagnosticArgument found(TokenKind value) { return make_argument(DiagnosticArgumentKind::Found, value); }

} // namespace diagnostic
