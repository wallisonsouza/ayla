#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/ast/statements/IfStatementNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"
#include "celestia/ast/statements/WhileStatementNode.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

#include <iostream>
namespace celestia::semantic {

void Resolver::if_statement(celestia::ast::IfStatement *node) {
  if (node->condition) { resolve_node(node->condition); }

  if (node->then_block) { resolve_node(node->then_block); }

  if (node->else_block) { resolve_node(node->else_block); }
}

void Resolver::while_statement(celestia::ast::WhileStatement *node) {
  if (node->condition) { resolve_node(node->condition); }

  if (node->body) { resolve_node(node->body); }
}

void Resolver::expression_statement(celestia::ast::ExpressionStatement *node) {

  if (node->expression) { resolve_node(node->expression); }
}

void Resolver::block_statement(ast::BlockStatement *node) {

  if (!node) return;

  for (auto *stmt : node->items) {
    if (!stmt) continue;

    resolve_node(stmt);
  }
}

void Resolver::return_statement(celestia::ast::ReturnStatement *node) {

  if (node->value) { resolve_node(node->value); }
}

} // namespace celestia::semantic