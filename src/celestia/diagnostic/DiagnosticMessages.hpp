#pragma once

#include "celestia/diagnostic/DiagnosticCode.hpp"
#include <string_view>
#include <unordered_map>

namespace diagnostic {

struct DiagnosticMessage {
  std::string_view title;
  std::string_view text;
};

// Diagnostic messages
inline const std::unordered_map<DiagnosticCode, DiagnosticMessage> messages = {

    // ============================================================
    // Lexer
    // ============================================================

    {DiagnosticCode::InvalidCharacter,
     {
         .title = "invalid character",
         .text = "invalid character",
     }},

    {DiagnosticCode::InvalidEscapeSequence,
     {
         .title = "invalid escape sequence",
         .text = "invalid escape sequence",
     }},

    {DiagnosticCode::UnterminatedString,
     {
         .title = "unterminated string",
         .text = "unterminated string",
     }},

    {DiagnosticCode::UnterminatedComment,
     {
         .title = "unterminated comment",
         .text = "unterminated comment",
     }},

    // ============================================================
    // Parser
    // ============================================================

    {DiagnosticCode::Expected,
     {
         .title = "expected",
         .text = "expected {expected}, found {found}",
     }},

    {DiagnosticCode::Unexpected,
     {
         .title = "unexpected",
         .text = "unexpected {found}",
     }},

    {DiagnosticCode::UnexpectedEOF,
     {
         .title = "unexpected end of file",
         .text = "unexpected end of file",
     }},

    // ============================================================
    // Resolver
    // ============================================================

    {DiagnosticCode::UndefinedSymbol,
     {
         .title = "undefined symbol",
         .text = "undefined symbol '{name}'",
     }},

    {DiagnosticCode::RedefinedSymbol,
     {
         .title = "redefined symbol",
         .text = "redefinition of '{name}'",
     }},

    {DiagnosticCode::ShadowedSymbol,
     {
         .title = "shadowed symbol",
         .text = "declaration of '{name}' shadows another symbol",
     }},

    {DiagnosticCode::UnknownType,
     {
         .title = "unknown type",
         .text = "unknown type '{name}'",
     }},

      {DiagnosticCode::UnknownModule,
     {
         .title = "unknown module",
         .text = "unknown module '{name}'",
     }},

    {DiagnosticCode::NotAType,
     {
         .title = "not a type",
         .text = "'{name}' is not a type",
     }},

    {DiagnosticCode::UnknownGeneric,
     {
         .title = "unknown generic",
         .text = "unknown generic '{name}'",
     }},

    {DiagnosticCode::InvalidGenericArity,
     {
         .title = "invalid generic arity",
         .text = "invalid number of generic arguments",
     }},

    // ============================================================
    // Type checker
    // ============================================================

    {DiagnosticCode::TypeMismatch,
     {
         .title = "type mismatch",
         .text = "type mismatch: expected {expected}, found {found}",
     }},

    {DiagnosticCode::InvalidAssignment,
     {
         .title = "invalid assignment",
         .text = "invalid assignment",
     }},

    {DiagnosticCode::InvalidConversion,
     {
         .title = "invalid conversion",
         .text = "invalid conversion",
     }},
};

// Help messages
inline const std::unordered_map<HelpCode, DiagnosticMessage> help_messages = {

    {HelpCode::UseReturnArrow,
     {
         .title = "help",
         .text = "function return types use '->'",
     }},

    {HelpCode::AddTypeAnnotation,
     {
         .title = "help",
         .text = "add ':' before the parameter type",
     }},

    {HelpCode::RenameSymbol,
     {
         .title = "help",
         .text = "consider renaming this symbol",
     }},
};

// Note messages
inline const std::unordered_map<NoteCode, DiagnosticMessage> note_messages = {

    {NoteCode::PreviousDeclaration,
     {
         .title = "note",
         .text = "previous declaration is here",
     }},

    {NoteCode::TypeDeclaredHere,
     {
         .title = "note",
         .text = "type declared here",
     }},

    {NoteCode::SymbolDeclaredHere,
     {
         .title = "note",
         .text = "symbol declared here",
     }},
};

} // namespace diagnostic