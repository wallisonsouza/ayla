#pragma once

#include <string_view>

enum class SymbolKind { Type, Capability, EnumVariant, Enum, Parameter, GenericParameter, Struct, Field, Function, Variable, Module };

constexpr std::string_view symbol_kind_name(SymbolKind kind) {
  switch (kind) {
  case SymbolKind::Type: return "type";
  case SymbolKind::Capability: return "capability";
  case SymbolKind::EnumVariant: return "enum variant";
  case SymbolKind::Enum: return "enum";
  case SymbolKind::Parameter: return "parameter";
  case SymbolKind::GenericParameter: return "generic parameter";
  case SymbolKind::Struct: return "struct";
  case SymbolKind::Field: return "field";
  case SymbolKind::Function: return "function";
  case SymbolKind::Variable: return "variable";
  case SymbolKind::Module: return "module";
  }

  return "unknown";
}