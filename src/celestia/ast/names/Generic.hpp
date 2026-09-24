

#pragma once

#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {


class GenericParameter : public Node {
public:
  Identifier *name;
  std::vector<Type *> constraints;

  GenericParameter(Identifier *name, std::vector<Type *> constraints) : Node(NodeKind::GenericParameter), name(name), constraints(std::move(constraints)) {}
};


struct GenericName : NameNode {
  Identifier *name;
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

  GenericName(Identifier *name, std::vector<GenericParameter *> parameters = {}) : NameNode(NodeKind::GenericName), name(name), parameters(std::move(parameters)) {}
};

} // namespace celestia::ast
