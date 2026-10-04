
#pragma once
#include "celestia/ast/declaration/Declaration.hpp"
#include "celestia/ast/expression/Expression.hpp"
#include <vector>

namespace celestia::ast {

struct BlockExpression : Expression {

  std::vector<BlockItem *> items;
  
  Expression *value;

  BlockExpression(std::vector<BlockItem *> items, Expression *value) : Expression(NodeKind::BlockExpression), items(std::move(items)), value(value) {}
};

} // namespace celestia::ast