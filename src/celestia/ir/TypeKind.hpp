#pragma once

#include <cstdint>

namespace celestia::ir {

enum class TypeKind : uint8_t {
  Void,

  Bool,
  Char,

  Int,
  UInt,

  Int8,
  Int16,
  Int32,
  Int64,

  UInt8,
  UInt16,
  UInt32,
  UInt64,

  F32,
  F64,

  String,

  Pointer,
   Interface,
  Array,
  Struct,
  Function
};

constexpr const char *type_kind_name(TypeKind kind) {

  switch (kind) {

  case TypeKind::Void: return "void";

  case TypeKind::Bool: return "bool";
  case TypeKind::Char: return "char";

  case TypeKind::Int: return "int";
  case TypeKind::UInt: return "uint";

  case TypeKind::Int8: return "int8";
  case TypeKind::Int16: return "int16";
  case TypeKind::Int32: return "int32";
  case TypeKind::Int64: return "int64";

  case TypeKind::UInt8: return "uint8";
  case TypeKind::UInt16: return "uint16";
  case TypeKind::UInt32: return "uint32";
  case TypeKind::UInt64: return "uint64";

  case TypeKind::F32: return "f32";
  case TypeKind::F64: return "f64";

  case TypeKind::String: return "string";

  case TypeKind::Pointer: return "pointer";
  case TypeKind::Array: return "array";
  case TypeKind::Struct: return "struct";
  case TypeKind::Function: return "function";
  case TypeKind::Interface: break;
  }

  return "unknown";
}

} // namespace celestia::ir