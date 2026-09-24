#pragma once

#include "Declaration.hpp"

#include "celestia/ast/names/Generic.hpp"
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/types/Type.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"

#include <utility>
#include <vector>

namespace celestia::ast {

struct FieldDeclaration : Declaration {

  Identifier *name;
  Type *type;

  FieldDeclaration(Identifier *name, Type *type) : Declaration(NodeKind::FieldDeclaration), name(name), type(type) {}
};

struct StructDeclaration : Declaration {

  Identifier *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  std::vector<Type *> compositions;

  std::vector<FieldDeclaration *> fields;

  StructDeclaration(
      Identifier *name,
      std::vector<GenericParameter *> parameters,
      std::vector<Type *> compositions,
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