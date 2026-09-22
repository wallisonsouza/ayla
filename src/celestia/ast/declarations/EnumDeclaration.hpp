#pragma once
#include "celestia/ast/Node.hpp"
#include "celestia/ast/declarations/Declaration.hpp"
#include "celestia/ast/names/GenericIdentifierNode.hpp"
#include "celestia/ast/names/IdentifierNode.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"
#include <vector>

namespace celestia::ast {

struct EnumVariant : Node {

  IdentifierNode *name;
  std::vector<TypeNode *> arguments;

  EnumVariant(IdentifierNode *name, std::vector<TypeNode *> args) : Node(NodeKind::EnumVariant), name(name), arguments(std::move(args)) {}
};

struct EnumDeclaration : Declaration {

  IdentifierNode *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  std::vector<EnumVariant *> variants;

  EnumDeclaration(IdentifierNode *name, std::vector<GenericParameter *> generic_parameters, std::vector<EnumVariant *> variants, DeclarationSpecifiers specifiers)
      : Declaration(NodeKind::EnumDeclaration), name(name), generic_parameters(std::move(generic_parameters)), specifiers(specifiers), variants(std::move(variants)) {}
};

} // namespace celestia::ast