#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "celestia/ir/Type.hpp"

namespace celestia::ir {

struct TypeHash {

  std::size_t operator()(const Type *type) const { return type->hash(); }
};

struct TypeEqual {

  bool operator()(const Type *a, const Type *b) const { return a->equals(*b); }
};

class IRGlobalContext {
private:
  std::vector<std::string> strings;
  std::unordered_map<std::string, StringId> string_ids;

  std::vector<std::unique_ptr<Type>> types;
  std::unordered_map<Type *, TypeId, TypeHash, TypeEqual> type_ids;

public:
  StringId intern_string(std::string value) {

    auto it = string_ids.find(value);

    if (it != string_ids.end()) return it->second;

    StringId id{static_cast<uint32_t>(strings.size())};

    strings.push_back(std::move(value));

    string_ids.emplace(strings.back(), id);

    return id;
  }

  const std::string &get_string(StringId id) const { return strings[id.index()]; }

  // Types
  TypeId create_type(std::unique_ptr<Type> type) {

    if (!type) return TypeId::invalid();

    TypeId id{static_cast<uint32_t>(types.size())};

    Type *ptr = type.get();

    types.push_back(std::move(type));

    type_ids.emplace(ptr, id);

    return id;
  }

  TypeId get_or_create_type(std::unique_ptr<Type> type) {

    if (!type) return TypeId::invalid();

    auto it = type_ids.find(type.get());

    if (it != type_ids.end()) return it->second;

    return create_type(std::move(type));
  }

  TypeId create_primitive_type(TypeKind kind) { return get_or_create_type(std::make_unique<PrimitiveType>(kind)); }

  TypeId create_array_type(TypeId element_type) {

    if (!element_type.is_valid()) return TypeId::invalid();

    return get_or_create_type(std::make_unique<ArrayType>(element_type));
  }

  TypeId create_pointer_type(TypeId pointee) {

    if (!pointee.is_valid()) return TypeId::invalid();

    return get_or_create_type(std::make_unique<PointerType>(pointee));
  }

  TypeId create_struct_type(ModuleId module, StructId id) {

    if (!module.is_valid() || !id.is_valid()) return TypeId::invalid();

    return get_or_create_type(std::make_unique<StructType>(module, id));
  }

  TypeId create_interface_type(ModuleId module, InterfaceId id) {

    if (!module.is_valid() || !id.is_valid()) return TypeId::invalid();

    return get_or_create_type(std::make_unique<InterfaceType>(module, id));
  }

  TypeId create_function_type(std::vector<TypeId> parameters, TypeId return_type) {

    if (!return_type.is_valid()) return TypeId::invalid();

    return get_or_create_type(std::make_unique<FunctionType>(std::move(parameters), return_type));
  }

  // ==================================================
  // Accessors
  // ==================================================

  Type &get_type(TypeId id) { return *types[id.index()]; }

  const Type &get_type(TypeId id) const { return *types[id.index()]; }

  // ==================================================
  // Counts
  // ==================================================

  size_t type_count() const { return types.size(); }
};

} // namespace celestia::ir