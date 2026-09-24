#pragma once
#include "celestia/ast/Node.hpp"
#include "celestia/ast/declarations/Declaration.hpp"
#include "celestia/ast/names/Generic.hpp"
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"
#include <vector>

namespace celestia::ast {

struct EnumVariant : Node {

  Identifier *name;
  std::vector<Type *> payload;

  EnumVariant(Identifier *name, std::vector<Type *> args) : Node(NodeKind::EnumVariant), name(name), payload(std::move(args)) {}
};

struct EnumDeclaration : Declaration {

  Identifier *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  std::vector<EnumVariant *> variants;

  EnumDeclaration(Identifier *name, std::vector<GenericParameter *> generic_parameters, std::vector<EnumVariant *> variants, DeclarationSpecifiers specifiers)
      : Declaration(NodeKind::EnumDeclaration), name(name), generic_parameters(std::move(generic_parameters)), specifiers(specifiers), variants(std::move(variants)) {}
};

} // namespace celestia::ast