#pragma once

#include "Declaration.hpp"

#include "celestia/ast/names/GenericIdentifierNode.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"


namespace celestia::ast {

struct TypeDeclaration : Declaration {

  IdentifierNode *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  TypeDeclaration(
      IdentifierNode *name,
      std::vector<GenericParameter *> generic_parameters,
      DeclarationSpecifiers specifiers)

      : Declaration(NodeKind::TypeDeclaration),
        name(name),
        generic_parameters(std::move(generic_parameters)),
        specifiers(specifiers) {}

};

} // namespace celestia::ast

