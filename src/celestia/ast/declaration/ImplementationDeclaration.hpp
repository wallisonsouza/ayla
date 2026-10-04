#pragma once

#include "Declaration.hpp"
#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {

struct ImplDeclaration : Declaration {

  Type *target;
  Type *capability;
  std::vector<FunctionDeclaration *> members;

  ImplDeclaration(Type *target, Type *capability, std::vector<FunctionDeclaration *> members)
      : Declaration(NodeKind::ImplementationDeclaration), target(target), capability(capability), members(std::move(members)) {}
};

} // namespace celestia::ast