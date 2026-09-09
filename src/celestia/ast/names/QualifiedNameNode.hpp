#pragma once

#include "celestia/ast/names/IdentifierNode.hpp"
#include "celestia/ast/names/NameNode.hpp"
#include <string>
#include <vector>

namespace celestia::ast {

struct QualifiedNameNode : NameNode {

  std::vector<IdentifierNode *> parts;

  std::string get_str() const override {
    std::string key;

    for (size_t i = 0; i < parts.size(); i++) {
      if (i > 0) key += ".";

      key += parts[i]->str;
    }

    return key;
  }

  QualifiedNameNode(std::vector<IdentifierNode *> parts) : NameNode(NodeKind::QualifiedName), parts(std::move(parts)) {}
};

} // namespace celestia::ast