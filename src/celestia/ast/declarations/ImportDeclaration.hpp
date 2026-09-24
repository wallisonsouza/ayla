#pragma once

#include "Declaration.hpp"
#include "celestia/ast/names/Name.hpp"
#include <optional>
#include <filesystem>

namespace celestia::ast {

struct ImportDeclaration : Declaration {
  NameNode *name;
  std::optional<std::string> path;
  std::optional<std::string> alias;

  explicit ImportDeclaration(NameNode *n, std::optional<std::string> p = std::nullopt, std::optional<std::string> a = std::nullopt)
      : Declaration(NodeKind::ImportDeclaration), name(n), path(std::move(p)), alias(std::move(a)) {}
};

} // namespace celestia::ast