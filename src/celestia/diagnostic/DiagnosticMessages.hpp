#pragma once

#include "celestia/diagnostic/Diagnostic.hpp"

#include <iostream>
#include <string_view>
#include <unordered_map>

namespace diagnostic {

struct DiagnosticMessage {
  std::string_view origin;
  std::string_view title;
  std::string_view text;
};

inline const std::unordered_map<DiagnosticCode, DiagnosticMessage> messages = {

    // ============================================================
    // Lexer
    // ============================================================

    {DiagnosticCode::InvalidCharacter,
     {
         .origin = "LEXER",
         .title = "invalid character",
         .text = "invalid character",
     }},

    {DiagnosticCode::InvalidEscapeSequence,
     {
         .origin = "LEXER",
         .title = "invalid escape sequence",
         .text = "invalid escape sequence",
     }},

    {DiagnosticCode::UnterminatedString,
     {
         .origin = "LEXER",
         .title = "unterminated string",
         .text = "unterminated string",
     }},

    {DiagnosticCode::UnterminatedComment,
     {
         .origin = "LEXER",
         .title = "unterminated comment",
         .text = "unterminated comment",
     }},

    // ============================================================
    // Parser
    // ============================================================

    {DiagnosticCode::Expected,
     {
         .origin = "PARSER",
         .title = "expected",
         .text = "expected {expected}, found {found}",
     }},

    {DiagnosticCode::Unexpected,
     {
         .origin = "PARSER",
         .title = "unexpected",
         .text = "unexpected {found}",
     }},

    {DiagnosticCode::UnexpectedEOF,
     {
         .origin = "PARSER",
         .title = "unexpected end of file",
         .text = "unexpected end of file",
     }},

    // ============================================================
    // Resolver
    // ============================================================

    {DiagnosticCode::UndefinedSymbol,
     {
         .origin = "RESOLVER",
         .title = "undefined symbol",
         .text = "undefined symbol '{name}'",
     }},

    {DiagnosticCode::UnknownModule,
     {
         .origin = "RESOLVER",
         .title = "unknown module",
         .text = "unknown module '{name}'",
     }},

    {DiagnosticCode::RedefinedSymbol,
     {
         .origin = "RESOLVER",
         .title = "redefined symbol",
         .text = "redefinition of {symbol}",
     }},

    {DiagnosticCode::RedefinedField,
     {
         .origin = "RESOLVER",
         .title = "redefined field",
         .text = "redefinition of field {symbol} '{name}'",
     }},

    {DiagnosticCode::ShadowedSymbol,
     {
         .origin = "RESOLVER",
         .title = "shadowed symbol",
         .text = "declaration of '{name}' shadows another symbol",
     }},

    {DiagnosticCode::UnknownType,
     {
         .origin = "RESOLVER",
         .title = "unknown type",
         .text = "unknown type '{name}'",
     }},

    {DiagnosticCode::NotAType,
     {
         .origin = "RESOLVER",
         .title = "not a type",
         .text = "'{name}' is not a type",
     }},

    {DiagnosticCode::UnknownGeneric,
     {
         .origin = "RESOLVER",
         .title = "unknown generic",
         .text = "unknown generic '{name}'",
     }},

    {DiagnosticCode::InvalidGenericArity,
     {
         .origin = "RESOLVER",
         .title = "invalid generic arity",
         .text = "invalid number of generic arguments",
     }},

    // ============================================================
    // Type checker
    // ============================================================

    {DiagnosticCode::TypeMismatch,
     {
         .origin = "TYPE_CHECKER",
         .title = "type mismatch",
         .text = "type mismatch",
     }},

    {DiagnosticCode::InvalidBinaryOperation,
     {
         .origin = "TYPE_CHECKER",
         .title = "invalid operation",
         .text = "invalid operation: '{lhs} {operator} {rhs}'",
     }},

    {DiagnosticCode::CannotInferType,
     {
         .origin = "TYPE_CHECKER",
         .title = "cannot infer type",
         .text = "cannot infer type: '{name}'",
     }},

    {DiagnosticCode::InvalidType,
     {
         .origin = "TYPE_CHECKER",
         .title = "invalid type",
         .text = "invalid type",
     }},

    {DiagnosticCode::InvalidAssignment,
     {
         .origin = "TYPE_CHECKER",
         .title = "invalid assignment",
         .text = "invalid assignment",
     }},

    {DiagnosticCode::InvalidConversion,
     {
         .origin = "TYPE_CHECKER",
         .title = "invalid conversion",
         .text = "invalid conversion",
     }},
};

inline DiagnosticMessage get_message(DiagnosticCode code) {
  const auto it = messages.find(code);

  if (it == messages.end()) {
    std::cerr << "[Diagnostic] missing message for code: " << static_cast<std::uint32_t>(code) << '\n';
    return {};
  }

  return it->second;
}

inline const std::unordered_map<LabelCode, DiagnosticMessage> label_messages = {

    {LabelCode::ExpectedType,
     {
         .origin = "TYPE_CHECKER",
         .title = "expected type",
         .text = "expected '{type}'",
     }},

    {LabelCode::LeftOperand,
     {
         .origin = "TYPE_CHECKER",
         .title = "left operand",
         .text = "has type '{type}'",
     }},

    {LabelCode::RightOperand,
     {
         .origin = "TYPE_CHECKER",
         .title = "right operand",
         .text = "has type '{type}'",
     }},

    {LabelCode::FoundType,
     {
         .origin = "TYPE_CHECKER",
         .title = "found",
         .text = "found '{type}'",
     }},

    {LabelCode::PreviousDeclaration,
     {
         .origin = "RESOLVER",
         .title = "previous declaration",
         .text = "previous declaration is here",
     }},

    {LabelCode::ConflictingDeclaration,
     {
         .origin = "RESOLVER",
         .title = "conflicting declaration",
         .text = "conflicting declaration is here",
     }},
};

} // namespace diagnostic