#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"
#include "celestia/syntax/parser/Parser.hpp"

namespace celestia::syntax {

celestia::ast::Statement *Parser::parse_statement() {

  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::RETURN_KEYWORD: return parse_return_statement();

  default: return nullptr;
  }
}

// return
celestia::ast::ReturnStatement *Parser::parse_return_statement() {
  auto &tokens = context.tokens();

  tokens.match(TokenKind::RETURN_KEYWORD);

  // return vazio
  if (tokens.check(TokenKind::CLOSE_BRACE) || tokens.check(TokenKind::NEW_LINE)) { return context.get_ast().alloc<celestia::ast::ReturnStatement>(nullptr); }

  auto *value = parse_expression();

  if (!value) return nullptr;

  return context.get_ast().alloc<celestia::ast::ReturnStatement>(value);
}

// exp
celestia::ast::ExpressionStatement *Parser::parse_expression_statement() {
  auto *expr = parse_expression();

  if (!expr) return nullptr;

  return context.get_ast().alloc<celestia::ast::ExpressionStatement>(expr);
}

} // namespace celestia::syntax
