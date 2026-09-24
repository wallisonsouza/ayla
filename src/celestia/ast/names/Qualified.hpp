#pragma once

#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/names/Name.hpp"
#include <string>
#include <vector>

namespace celestia::ast {

struct QualifiedName : NameNode {

  std::vector<Identifier *> parts;

  std::string get_str() const override {
    std::string key;

    for (size_t i = 0; i < parts.size(); i++) {
      if (i > 0) key += ".";

      key += parts[i]->str;
    }

    return key;
  }

  QualifiedName(std::vector<Identifier *> parts) : NameNode(NodeKind::QualifiedName), parts(std::move(parts)) {}
};

} // namespace celestia::ast