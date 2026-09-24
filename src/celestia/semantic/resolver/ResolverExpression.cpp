#include "celestia/semantic/resolver/Resolver.hpp"

#include "celestia/ast/expressions/AssignmentExpression.hpp"
#include "celestia/ast/expressions/BinaryExpressionNode.hpp"
#include "celestia/ast/expressions/CallExpressionNode.hpp"
#include "celestia/ast/expressions/IdentifierExpressionNode.hpp"
#include "celestia/ast/expressions/IndexAcessExpressionNode.hpp"
#include "celestia/ast/expressions/MemberAccessExpressionNode.hpp"
#include "celestia/ast/expressions/UnaryExpressionNode.hpp"

namespace celestia::semantic {


void Resolver::resolve_identifier_expression(ast::IdentifierExpressionNode *node) {
  auto current = context.stack.current();

  debug::Trace::header(debug::Category::Resolver, "Resolving '{}' in scope {}", node->name->get_str(), current.index());

  ScopeId scope_id = context.stack.current();

  if (!scope_id.is_valid()) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::NotAType,
        .arguments =
            {
                diagnostic::name(node->name->str),
            },
    });

    return;
  }

  SymbolId id = context.get_env().scopes.lookup(scope_id, node->name->str);

  if (!id.is_valid()) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::UndefinedSymbol,
        .arguments =
            {
                diagnostic::name(node->name->str),
            },
    });

    return;
  }

  context.unit.semantic.set_symbol(node, id);
}

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