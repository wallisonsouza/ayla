#pragma once
#include "Declaration.hpp"
#include "celestia/ast/statements/BlockStatementNode.hpp"

namespace celestia::ast {

struct ModuleInitDeclaration : Declaration {

  BlockStatement *body = nullptr;

  explicit ModuleInitDeclaration(BlockStatement *body) : Declaration(NodeKind::ModuleInitDeclaration), body(body) {}
};

struct ModuleDeclaration : Declaration {

  NameNode *name;

  std::vector<Declaration *> declarations;

  ModuleDeclaration(NameNode *n, std::vector<Declaration *> decls = {}) : Declaration(NodeKind::ModuleDeclaration), name(n), declarations(std::move(decls)) {}
};

} // namespace celestia::ast
