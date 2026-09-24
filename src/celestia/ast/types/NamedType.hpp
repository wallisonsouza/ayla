#pragma once

#include "celestia/ast/names/Name.hpp"
#include "celestia/ast/types/Type.hpp"

namespace celestia::ast {

struct NamedType : Type {

  NameNode *name;

  NamedType(NameNode *name, bool primitive = false) : Type(NodeKind::NamedType), name(name) {}
};

} // namespace celestia::ast