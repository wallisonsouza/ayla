#pragma once

#include "Declaration.hpp"

#include "celestia/ast/names/GenericIdentifierNode.hpp"
#include "celestia/ast/names/IdentifierNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"

#include <utility>
#include <vector>

namespace celestia::ast {

struct FieldDeclaration : Declaration {

  IdentifierNode *name;
  TypeNode *type;

  FieldDeclaration(IdentifierNode *name, TypeNode *type) : Declaration(NodeKind::FieldDeclaration), name(name), type(type) {}
};

struct StructDeclaration : Declaration {

  IdentifierNode *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  std::vector<TypeNode *> compositions;

  std::vector<FieldDeclaration *> fields;

  StructDeclaration(
      IdentifierNode *name,
      std::vector<GenericParameter *> parameters,
      std::vector<TypeNode *> compositions,
      std::vector<FieldDeclaration *> fields,
      DeclarationSpecifiers specifiers)

      : Declaration(NodeKind::StructDeclaration),
        name(name),
        generic_parameters(std::move(parameters)),
        specifiers(specifiers),
        compositions(std::move(compositions)),
        fields(std::move(fields)) {}
};

} // namespace celestia::ast