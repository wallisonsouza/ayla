#pragma once

#include "Declaration.hpp"

#include "celestia/ast/names/GenericIdentifierNode.hpp"
#include "celestia/ast/patterns/PatternNode.hpp"
#include "celestia/ast/statements/BlockStatementNode.hpp"
#include "celestia/ast/types/TypeNode.hpp"

#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"

#include <utility>
#include <vector>

namespace celestia::ast {

struct FunctionDeclaration : Declaration {

  IdentifierNode *name;

  std::vector<GenericParameter *> generic_parameters;

  DeclarationSpecifiers specifiers;

  std::vector<PatternNode *> parameters;

  TypeNode *return_type;

  BlockStatement *body;

  FunctionDeclaration(
      IdentifierNode *name = nullptr,
      std::vector<GenericParameter *> generic_parameters = {},
      std::vector<PatternNode *> parameters = {},
      TypeNode *return_type = nullptr,
      BlockStatement *body = nullptr,
      DeclarationSpecifiers specifiers = {})

      : Declaration(NodeKind::FunctionDeclaration),
        name(name),
        generic_parameters(std::move(generic_parameters)),
        specifiers(specifiers),
        parameters(std::move(parameters)),
        return_type(return_type),
        body(body) {}
};
} // namespace celestia::ast