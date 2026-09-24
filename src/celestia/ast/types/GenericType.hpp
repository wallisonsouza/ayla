#pragma once

#include "celestia/ast/names/Name.hpp"
#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {

struct GenericType : Type {

  NameNode *name;
  std::vector<Type *> arguments;

  GenericType(NameNode *name, std::vector<Type *> arguments) : Type(NodeKind::GenericType), name(name), arguments(std::move(arguments)) {}
};
} // namespace celestia::ast