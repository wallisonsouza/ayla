#pragma once

#include "celestia/ast/declarations/Declaration.hpp"
#include "celestia/ast/statements/StatementNode.hpp"
#include <vector>

namespace celestia::ast {

struct BlockStatement : Statement {
  std::vector<BlockItem *> items;

  explicit BlockStatement(std::vector<BlockItem *> stmts = {}) : Statement(NodeKind::BlockStatement), items(std::move(stmts)) {}
};
} // namespace celestia::ast
