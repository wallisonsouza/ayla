#pragma once

#include "Declaration.hpp"

#include "celestia/ast/names/GenericIdentifierNode.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"

#include <utility>
#include <vector>

namespace celestia::ast {

struct CapabilityDeclaration : Declaration {

  IdentifierNode *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  std::vector<Declaration *> members;

  CapabilityDeclaration(
      IdentifierNode *name,
      std::vector<GenericParameter *> generic_parameters,
      std::vector<Declaration *> members,
      DeclarationSpecifiers specifiers)

      : Declaration(NodeKind::CapabilityDeclaration),
        name(name),
        generic_parameters(std::move(generic_parameters)),
        specifiers(specifiers),
        members(std::move(members)) {}
};

} // namespace celestia::ast