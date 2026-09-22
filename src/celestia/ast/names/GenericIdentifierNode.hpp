

#pragma once

#include "celestia/ast/names/IdentifierNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"
#include <vector>

namespace celestia::ast {


class GenericParameter : public Node {
public:
  IdentifierNode *name;
  std::vector<TypeNode *> constraints;

  GenericParameter(IdentifierNode *name, std::vector<TypeNode *> constraints) : Node(NodeKind::GenericParameter), name(name), constraints(std::move(constraints)) {}
};


struct GenericIdentifierNode : NameNode {
  IdentifierNode *name;
  std::vector<GenericParameter *> parameters;

  std::string get_str() const override {
    std::string result = name->get_str();

    if (!parameters.empty()) {
      result += "<";

      for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) result += ", ";

        result += parameters[i]->name->get_str();
      }

      result += ">";
    }

    return result;
  }

  GenericIdentifierNode(IdentifierNode *name, std::vector<GenericParameter *> parameters = {}) : NameNode(NodeKind::GenericIdentifier), name(name), parameters(std::move(parameters)) {}
};

} // namespace celestia::ast
