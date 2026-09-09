#pragma once

#include "Declaration.hpp"
#include "celestia/ast/names/QualifiedNameNode.hpp"

#include <optional>

namespace celestia::ast {

struct ImportDeclaration : Declaration {
  NameNode* name;
  std::optional<std::string> alias;

  explicit ImportDeclaration(NameNode* n) : Declaration(NodeKind::ImportDeclaration), name(n) {}


};
} // namespace celestia::ast
