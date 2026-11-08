#include "Parser.hpp"
#include "ParserContext.hpp"
#include "celestia/syntax/parser/Parser.hpp"

#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/syntax/parser/ParseStatus.hpp"

namespace celestia::syntax {

Match<ast::Type *> Parser::parse_type() {

  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::OPEN_PAREN: return upcast<ast::Type>(parse_function_type());

  case TokenKind::IDENTIFIER: {

    // Parse the complete name first.
    //
    // Int
    // foo.Int
    // Vec<Int>
    // foo.Vec<Int>
    auto name_result = parse_name();

    if (name_result.is_error()) { return Match<ast::Type *>::fail(); }

    if (name_result.is_no_match()) { return Match<ast::Type *>::no_match(); }

    auto *name = name_result.value();

    // NamedType or GenericType
    if (tokens.check(TokenKind::LESS)) { return upcast<ast::Type>(parse_generic_type(name)); }

    auto *type = context.get_ast().alloc<ast::NamedType>(name);

    type->slice = name->slice;

    return Match<ast::Type *>::ok(type);
  }

  default: return Match<ast::Type *>::no_match();
  }
}

Match<ast::NamedType *> Parser::parse_named_type() {

  auto result = parse_name();

  if (result.is_error()) { return Match<ast::NamedType *>::fail(); }

  if (result.is_no_match()) {
    parser::diagnostics::report_expected_type(context);
    return Match<ast::NamedType *>::fail();
  }

  auto *name = result.value();

  auto *type = context.get_ast().alloc<ast::NamedType>(name);

  type->slice = name->slice;

  return Match<ast::NamedType *>::ok(type);
}

Match<ast::GenericType *>
Parser::parse_generic_type(ast::NameNode *name) {

  auto &tokens = context.tokens();

  auto result =
      parse_delimited_list<ast::Type *>(
          context,
          TokenKind::LESS,
          TokenKind::GREATER,
          TokenKind::COMMA,
          [&]() -> Match<ast::Type *> {
            return parse_type();
          }
      );

  if (result.is_error()) {
    return Match<ast::GenericType *>::fail();
  }

  if (result.is_no_match()) {
    return Match<ast::GenericType *>::no_match();
  }

  auto arguments = std::move(result.value());

  // Type<>
  if (arguments.empty()) {
    parser::diagnostics::report_expected_type(context);
    return Match<ast::GenericType *>::fail();
  }

  auto *type =
      context.get_ast().alloc<ast::GenericType>(
          name,
          std::move(arguments)
      );

  type->slice = name->slice;

  return Match<ast::GenericType *>::ok(type);
}

Match<ast::FunctionType *> Parser::parse_function_type() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) { return Match<ast::FunctionType *>::no_match(); }

  std::vector<ast::Type *> parameters;

  tokens.skip_trivia();

  // () -> Type
  if (!tokens.match(TokenKind::CLOSE_PAREN)) {

    while (!tokens.is_end()) {

      // (Type, ...)
      auto result = parse_type();

      if (result.is_error()) { return Match<ast::FunctionType *>::fail(); }

      if (result.is_no_match()) {

        parser::diagnostics::report_expected_type(context);

        return Match<ast::FunctionType *>::fail();
      }

      parameters.push_back(result.value());

      tokens.skip_trivia();

      // (Type)
      if (tokens.match(TokenKind::CLOSE_PAREN)) { break; }

      // (Type, ...)
      if (!tokens.match(TokenKind::COMMA)) {

        parser::diagnostics::report_expected(context, TokenKind::COMMA);

        return Match<ast::FunctionType *>::fail();
      }

      tokens.skip_trivia();

      // (Type,)
      if (tokens.match(TokenKind::CLOSE_PAREN)) { break; }
    }

    if (tokens.is_end()) {

      parser::diagnostics::report_expected(context, TokenKind::CLOSE_PAREN);

      return Match<ast::FunctionType *>::fail();
    }
  }

  // () ...
  if (!tokens.match(TokenKind::ARROW)) {

    parser::diagnostics::report_expected(context, TokenKind::ARROW);

    return Match<ast::FunctionType *>::fail();
  }

  // () -> ...
  auto return_result = parse_type();

  if (return_result.is_error()) { return Match<ast::FunctionType *>::fail(); }

  if (return_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return Match<ast::FunctionType *>::fail();
  }

  auto *type = context.get_ast().alloc<ast::FunctionType>(std::move(parameters), return_result.value());

  auto end = tokens.position();

  return Match<ast::FunctionType *>::ok(type);
}

} // namespace celestia::syntax