#pragma once

#include "Declaration.hpp"
#include "celestia/ast/ASTFwd.hpp"
#include <vector>

namespace celestia::ast {

struct ImplDeclaration : Declaration {

  TypeNode *target;
  TypeNode *capability;
  std::vector<FunctionDeclaration *> members;

  ImplDeclaration(TypeNode *target, TypeNode *capability, std::vector<FunctionDeclaration *> members)
      : Declaration(NodeKind::ImplementationDeclaration), target(target), capability(capability), members(std::move(members)) {}
};

} // namespace celestia::ast