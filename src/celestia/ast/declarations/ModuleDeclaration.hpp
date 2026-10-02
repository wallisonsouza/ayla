#pragma once
#include "Declaration.hpp"
#include "celestia/ast/expressions/BlockExpression.hpp"
#include "celestia/ast/names/Name.hpp"

namespace celestia::ast {

struct ModuleInitDeclaration : Declaration {

  BlockExpression *body = nullptr;

  explicit ModuleInitDeclaration(BlockExpression *body) : Declaration(NodeKind::ModuleInitDeclaration), body(body) {}
};

struct ModuleDeclaration : Declaration {

  NameNode *name;

  std::vector<Declaration *> declarations;

  ModuleDeclaration(NameNode *n, std::vector<Declaration *> decls = {}) : Declaration(NodeKind::ModuleDeclaration), name(n), declarations(std::move(decls)) {}
};

} // namespace celestia::ast
