#include "celestia/ast/expressions/UnaryExpressionNode.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
namespace celestia::semantic {
void Resolver::binary_expression(celestia::ast::BinaryExpressionNode *node) {
  resolve_node(node->lhs);
  resolve_node(node->rhs);
}

void Resolver::unary_expression(celestia::ast::UnaryExpressionNode *node) {
  //
  if (node->operand) { resolve_node(node->operand); }
}

void Resolver::assignment(celestia::ast::AssignmentExpressionNode *node) {

  if (!node) return;

  if (node->target) resolve_node(node->target);

  auto symbol_id = context.unit.semantic.symbol(node->target);

  if (!symbol_id.is_valid()) {
    std::cout << "ERRO: assignment target sem simbolo\n";
    return;
  }

  auto &symbol = context.get_env().symbols.get(symbol_id);

  if (node->value) resolve_node(node->value);
}

void Resolver::index_access(celestia::ast::IndexAccessExpressionNode *node) {

  if (node->base) resolve_node(node->base);

  if (node->index) resolve_node(node->index);
}

void Resolver::member_access(celestia::ast::MemberAccessExpressionNode *node) {

  if (!node || !node->base || !node->member) return;

  resolve_node(node->base);

  auto base_symbol = context.unit.semantic.symbol(node->base);

  if (!base_symbol.is_valid()) {

    std::cout << "ERRO: membro acessado em simbolo invalido\n";

    return;
  }

  context.unit.semantic.set_symbol(node, base_symbol);
}
void Resolver::function_call(celestia::ast::CallExpressionNode *node) {

  if (node->callee) resolve_node(node->callee);

  for (auto *arg : node->arguments) { resolve_node(arg); }
}
} // namespace celestia::semantic