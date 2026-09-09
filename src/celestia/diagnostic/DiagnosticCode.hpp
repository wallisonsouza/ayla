#pragma once

#include <cstdint>

namespace diagnostic {

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
  ShadowedSymbol,
  UnknownType,
  NotAType,
  UnknownGeneric,
  InvalidGenericArity,

  // Type checker
  TypeMismatch,
  InvalidAssignment,
  InvalidConversion,
};

enum class HelpCode : std::uint32_t {
  None = 0,

  // Syntax
  UseReturnArrow,
  AddMissingDelimiter,
  AddTypeAnnotation,

  // Names
  RenameSymbol,
  UseQualifiedName,

  // Types
  SpecifyGenericArgument,
  AddExplicitConversion,
};

enum class NoteCode : std::uint32_t {
  None = 0,

  PreviousDeclaration,
  TypeDeclaredHere,
  SymbolDeclaredHere,
  RelatedExpression,
};
;
} // namespace diagnostic