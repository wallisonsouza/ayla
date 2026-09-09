#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include "celestia/ir/IRIds.hpp"
#include "celestia/ir/TypeKind.hpp"

namespace celestia::ir {

struct Type {

  ModuleId module;
  TypeKind kind;

  explicit Type(TypeKind kind) : module(ModuleId::invalid()), kind(kind) {}

  Type(ModuleId module, TypeKind kind) : module(module), kind(kind) {}

  virtual ~Type() = default;

  virtual bool equals(const Type &other) const = 0;
  virtual std::size_t hash() const = 0;
};

struct PrimitiveType : Type {

  explicit PrimitiveType(TypeKind kind) : Type(TypeKind::Void) { this->kind = kind; }

  bool equals(const Type &other) const override { return other.kind == kind; }

  std::size_t hash() const override { return static_cast<size_t>(kind); }
};

struct ArrayType : Type {

  TypeId element_type;

  explicit ArrayType(TypeId element_type) : Type(TypeKind::Array), element_type(element_type) {}

  bool equals(const Type &other) const override {

    if (other.kind != TypeKind::Array) return false;

    const auto &array = static_cast<const ArrayType &>(other);

    return element_type == array.element_type;
  }

  std::size_t hash() const override { return std::hash<uint32_t>{}(element_type.index()); }
};

struct PointerType : Type {

  TypeId pointee;

  explicit PointerType(TypeId pointee) : Type(TypeKind::Pointer), pointee(pointee) {}

  bool equals(const Type &other) const override {

    if (other.kind != TypeKind::Pointer) return false;

    const auto &pointer = static_cast<const PointerType &>(other);

    return pointee == pointer.pointee;
  }

  std::size_t hash() const override { return std::hash<uint32_t>{}(pointee.index()); }
};

struct StructType : Type {

  StructId id;

  StructType(ModuleId module, StructId id) : Type(module, TypeKind::Struct), id(id) {}

  bool equals(const Type &other) const override {

    if (other.kind != TypeKind::Struct) return false;

    const auto &structure = static_cast<const StructType &>(other);

    return module == structure.module && id == structure.id;
  }

  std::size_t hash() const override {

    std::size_t result = std::hash<uint32_t>{}(module.index());

    result ^= std::hash<uint32_t>{}(id.index()) + 0x9e3779b9 + (result << 6) + (result >> 2);

    return result;
  }
};

struct InterfaceType : Type {

  InterfaceId id;

  InterfaceType(ModuleId module, InterfaceId id) : Type(module, TypeKind::Interface), id(id) {}

  bool equals(const Type &other) const override {

    if (other.kind != TypeKind::Interface) return false;

    const auto &interface_type = static_cast<const InterfaceType &>(other);

    return module == interface_type.module && id == interface_type.id;
  }

  std::size_t hash() const override {

    std::size_t result = std::hash<uint32_t>{}(module.index());

    result ^= std::hash<uint32_t>{}(id.index()) + 0x9e3779b9 + (result << 6) + (result >> 2);

    return result;
  }
};

struct FunctionType : Type {

  std::vector<TypeId> parameters;
  TypeId return_type;

  FunctionType(std::vector<TypeId> parameters, TypeId return_type) : Type(TypeKind::Function), parameters(std::move(parameters)), return_type(return_type) {}

  bool equals(const Type &other) const override {

    if (other.kind != TypeKind::Function) return false;

    const auto &function = static_cast<const FunctionType &>(other);

    return parameters == function.parameters && return_type == function.return_type;
  }

  std::size_t hash() const override {

    std::size_t result = 0;

    for (TypeId parameter : parameters) { result ^= std::hash<uint32_t>{}(parameter.index()) + 0x9e3779b9 + (result << 6) + (result >> 2); }

    result ^= std::hash<uint32_t>{}(return_type.index()) + 0x9e3779b9 + (result << 6) + (result >> 2);

    return result;
  }
};

} // namespace celestia::ir