#pragma once

#include "celestia/core/operators/BinaryOperation.hpp"
#include "celestia/core/token/Location.hpp"
#include "celestia/core/token/Token.hpp"
#include "celestia/core/token/TokenKind.hpp"
#include "celestia/semantic/id/ids.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace diagnostic {

//--------------------------------------------------
// Diagnostic
//--------------------------------------------------

enum class DiagnosticCode : std::uint32_t {
  None = 0,

  // Lexer
  InvalidCharacter,
  InvalidEscapeSequence,
  UnterminatedString,
  UnterminatedComment,

  // Parser
  Expected,
  Unexpected,
  UnexpectedEOF,

  // Resolver
  UndefinedSymbol,
  UnknownModule,
  RedefinedSymbol,
  RedefinedField,
  ShadowedSymbol,
  UnknownType,
  NotAType,
  UnknownGeneric,
  InvalidGenericArity,

  // Type checker
  TypeMismatch,
  InvalidBinaryOperation,
  CannotInferType,
  InvalidType,
  InvalidAssignment,
  InvalidConversion,
};

//--------------------------------------------------
// Expected
//--------------------------------------------------

enum class ExpectedKind {
  Token,

  Identifier,

  Expression,
  Statement,
  Pattern,

  Type,

  Declaration
};

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



struct OperationTypes {
  celestia::semantic::TypeId lhs;
  celestia::semantic::TypeId rhs;
};

//--------------------------------------------------
// Diagnostic arguments
//--------------------------------------------------

using Argument = std::variant<OperationTypes, BinaryOperation, ExpectedToken, ExpectedCategory, Token *, TokenKind, celestia::semantic::TypeId, celestia::semantic::SymbolId, std::string>;

enum class LabelCode {
  None,

  ExpectedType,
  FoundType,
  Location,
  ExpectedHere,

  LeftOperand,
  RightOperand,

  PreviousDeclaration,
  ConflictingDeclaration,
};

struct Label {
  SourceSlice slice;
  LabelCode code;
  std::vector<Argument> arguments;
};

//--------------------------------------------------
// Severity
//--------------------------------------------------

enum class Severity {
  Error,
  Warning,
};

//--------------------------------------------------
// Diagnostic
//--------------------------------------------------

struct Diagnostic {
  Severity severity;
  DiagnosticCode code;

  SourceSlice primary_slice;

  std::vector<Argument> arguments;
  std::vector<Label> labels;
};

//--------------------------------------------------
// Labels
//--------------------------------------------------

inline Label label(SourceSlice slice, LabelCode code) {

  return {
      .slice = slice,
      .code = code,
      .arguments = {},
  };
}

inline Label label(SourceSlice slice, LabelCode code, std::vector<Argument> arguments) {

  return {
      .slice = slice,
      .code = code,
      .arguments = std::move(arguments),
  };
}

} // namespace diagnostic