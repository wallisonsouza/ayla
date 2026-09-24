#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "celestia/semantic/id/ids.hpp"
#include "celestia/semantic/types/PrimitiveType.hpp"
#include "celestia/semantic/types/TypeKind.hpp"

namespace celestia::semantic {

struct Type {
  TypeKind kind;

  explicit Type(TypeKind kind) : kind(kind) {}

  virtual ~Type() = default;

  virtual std::string to_string() const {
    switch (kind) {
    case TypeKind::Unknown: return "Unknown";

    case TypeKind::Primitive: return "Primitive";

    case TypeKind::Function: return "Function";

    case TypeKind::Object: return "Object";

    case TypeKind::Struct: return "Struct";

    case TypeKind::GenericInstance: return "GenericInstance";
    }

    return "Invalid";
  }
};

struct PrimitiveType : Type {
  PrimitiveKind primitive;

  explicit PrimitiveType(PrimitiveKind primitive) : Type(TypeKind::Primitive), primitive(primitive) {}

  std::string to_string() const override { return std::string(primitive_kind_name(primitive)); }
};

struct ObjectType : Type {
  std::unordered_map<std::string, TypeId> members;

  ObjectType() : Type(TypeKind::Object) {}

  bool has_member(const std::string &name) const { return members.find(name) != members.end(); }

  TypeId get_member(const std::string &name) const {
    auto it = members.find(name);

    if (it == members.end()) return TypeId::invalid();

    return it->second;
  }

  void add_member(const std::string &name, TypeId type) { members[name] = type; }
};

struct ModuleType : ObjectType {};

struct FunctionType : Type {
  std::vector<TypeId> params;
  TypeId return_type = TypeId::invalid();

  FunctionType() : Type(TypeKind::Function) {}
};

struct GenericDeclarationType : Type {
  SymbolId symbol;
  size_t arity;

  GenericDeclarationType(SymbolId symbol, size_t arity) : Type(TypeKind::Generic), symbol(symbol), arity(arity) {}
};

struct GenericInstanceType : Type {
  TypeId constructor;
  std::vector<TypeId> arguments;

  GenericInstanceType(TypeId constructor, std::vector<TypeId> arguments) : Type(TypeKind::GenericInstance), constructor(constructor), arguments(std::move(arguments)) {}
};

struct StructMember {
  std::string name;
  TypeId type;
};

struct StructType : Type {

  SymbolId symbol;

  std::vector<StructMember> members;

  std::unordered_map<std::string, size_t> member_lookup;

  explicit StructType(SymbolId symbol) : Type(TypeKind::Struct), symbol(symbol) {}

  bool has_member(const std::string &name) const { return member_lookup.find(name) != member_lookup.end(); }

  TypeId get_member(const std::string &name) const {

    auto it = member_lookup.find(name);

    if (it == member_lookup.end()) return TypeId::invalid();

    return members[it->second].type;
  }

  void add_member(std::string name, TypeId type) {

    const size_t index = members.size();

    members.push_back({std::move(name), type});

    member_lookup.emplace(members[index].name, index);
  }
};

} // namespace celestia::semantic