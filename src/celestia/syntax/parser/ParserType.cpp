#include "Parser.hpp"
#include "ParserContext.hpp"
#include "celestia/syntax/parser/Parser.hpp"

#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/syntax/parser/ParseStatus.hpp"

namespace celestia::syntax {

ParseResult<ast::TypeNode *> Parser::parse_type() {

  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::OPEN_PAREN: return upcast<ast::TypeNode>(parse_function_type());

  case TokenKind::IDENTIFIER: {

    // Parse the complete name first.
    //
    // Int
    // foo.Int
    // Vec<Int>
    // foo.Vec<Int>
    auto name_result = parse_name();

    if (name_result.is_error()) { return ParseResult<ast::TypeNode *>::fail(); }

    if (name_result.is_no_match()) { return ParseResult<ast::TypeNode *>::no_match(); }

    auto *name = name_result.value();

    // NamedType or GenericTypeNode
    if (tokens.check(TokenKind::LESS)) { return upcast<ast::TypeNode>(parse_generic_type(name)); }

    auto *type = context.get_ast().alloc<ast::NamedType>(name);

    type->slice = name->slice;

    return ParseResult<ast::TypeNode *>::ok(type);
  }

  default: return ParseResult<ast::TypeNode *>::no_match();
  }
}

ParseResult<ast::NamedType *> Parser::parse_named_type() {

  auto result = parse_name();

  if (result.is_error()) { return ParseResult<ast::NamedType *>::fail(); }

  if (result.is_no_match()) {
    parser::diagnostics::report_expected_type(context);
    return ParseResult<ast::NamedType *>::fail();
  }

  auto *name = result.value();

  auto *type = context.get_ast().alloc<ast::NamedType>(name);

  type->slice = name->slice;

  return ParseResult<ast::NamedType *>::ok(type);
}

ParseResult<ast::GenericTypeNode *> Parser::parse_generic_type(ast::NameNode *name) {

  auto &tokens = context.tokens();

  // Type<
  if (!tokens.match(TokenKind::LESS)) {

    parser::diagnostics::report_expected(context, TokenKind::LESS);

    return ParseResult<ast::GenericTypeNode *>::fail();
  }

  std::vector<ast::TypeNode *> arguments;

  tokens.skip_trivia();

  // Type<>
  if (tokens.match(TokenKind::GREATER)) {

    parser::diagnostics::report_expected_type(context);

    return ParseResult<ast::GenericTypeNode *>::fail();
  }

  while (!tokens.is_end()) {

    // Type<int, ...>
    auto result = parse_type();

    // O parser interno já gerou o diagnóstico.
    if (result.is_error()) { return ParseResult<ast::GenericTypeNode *>::fail(); }

    // Aqui um tipo é obrigatório.
    if (result.is_no_match()) {

      parser::diagnostics::report_expected_type(context);

      return ParseResult<ast::GenericTypeNode *>::fail();
    }

    arguments.push_back(result.value());

    tokens.skip_trivia();

    // Type<int>
    if (tokens.match(TokenKind::GREATER)) {

      auto *type = context.get_ast().alloc<ast::GenericTypeNode>(name, std::move(arguments));

      type->slice = name->slice;

      return ParseResult<ast::GenericTypeNode *>::ok(type);
    }

    // Type<int, ...>
    if (!tokens.match(TokenKind::COMMA)) {

      parser::diagnostics::report_expected(context, TokenKind::GREATER);

      return ParseResult<ast::GenericTypeNode *>::fail();
    }

    tokens.skip_trivia();

    // Type<int,>
    if (tokens.match(TokenKind::GREATER)) {

      parser::diagnostics::report_expected_type(context);

      return ParseResult<ast::GenericTypeNode *>::fail();
    }
  }

  // Type<int EOF
  parser::diagnostics::report_expected(context, TokenKind::GREATER);

  return ParseResult<ast::GenericTypeNode *>::fail();
}

ParseResult<ast::FunctionType *> Parser::parse_function_type() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) { return ParseResult<ast::FunctionType *>::no_match(); }

  std::vector<ast::TypeNode *> parameters;

  tokens.skip_trivia();

  // () -> Type
  if (!tokens.match(TokenKind::CLOSE_PAREN)) {

    while (!tokens.is_end()) {

      // (Type, ...)
      auto result = parse_type();

      if (result.is_error()) { return ParseResult<ast::FunctionType *>::fail(); }

      if (result.is_no_match()) {

        parser::diagnostics::report_expected_type(context);

        return ParseResult<ast::FunctionType *>::fail();
      }

      parameters.push_back(result.value());

      tokens.skip_trivia();

      // (Type)
      if (tokens.match(TokenKind::CLOSE_PAREN)) { break; }

      // (Type, ...)
      if (!tokens.match(TokenKind::COMMA)) {

        parser::diagnostics::report_expected(context, TokenKind::COMMA);

        return ParseResult<ast::FunctionType *>::fail();
      }

      tokens.skip_trivia();

      // (Type,)
      if (tokens.match(TokenKind::CLOSE_PAREN)) { break; }
    }

    if (tokens.is_end()) {

      parser::diagnostics::report_expected(context, TokenKind::CLOSE_PAREN);

      return ParseResult<ast::FunctionType *>::fail();
    }
  }

  // () ...
  if (!tokens.match(TokenKind::ARROW)) {

    parser::diagnostics::report_expected(context, TokenKind::ARROW);

    return ParseResult<ast::FunctionType *>::fail();
  }

  // () -> ...
  auto return_result = parse_type();

  if (return_result.is_error()) { return ParseResult<ast::FunctionType *>::fail(); }

  if (return_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return ParseResult<ast::FunctionType *>::fail();
  }

  auto *type = context.get_ast().alloc<ast::FunctionType>(std::move(parameters), return_result.value());

  auto end = tokens.position();

  return ParseResult<ast::FunctionType *>::ok(type);
}

} // namespace celestia::syntax