#include "Parser.hpp"
#include "ParserContext.hpp"
#include "celestia/syntax/parser/Parser.hpp"

#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/syntax/parser/ParseStatus.hpp"

namespace celestia::syntax {

ParseResult<ast::Type *> Parser::parse_type() {

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

    if (name_result.is_error()) { return ParseResult<ast::Type *>::fail(); }

    if (name_result.is_no_match()) { return ParseResult<ast::Type *>::no_match(); }

    auto *name = name_result.value();

    // NamedType or GenericType
    if (tokens.check(TokenKind::LESS)) { return upcast<ast::Type>(parse_generic_type(name)); }

    auto *type = context.get_ast().alloc<ast::NamedType>(name);

    type->slice = name->slice;

    return ParseResult<ast::Type *>::ok(type);
  }

  default: return ParseResult<ast::Type *>::no_match();
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

ParseResult<ast::GenericType *>
Parser::parse_generic_type(ast::NameNode *name) {

  auto &tokens = context.tokens();

  auto result =
      parse_delimited_list<ast::Type *>(
          context,
          TokenKind::LESS,
          TokenKind::GREATER,
          TokenKind::COMMA,
          [&]() -> ParseResult<ast::Type *> {
            return parse_type();
          }
      );

  if (result.is_error()) {
    return ParseResult<ast::GenericType *>::fail();
  }

  if (result.is_no_match()) {
    return ParseResult<ast::GenericType *>::no_match();
  }

  auto arguments = std::move(result.value());

  // Type<>
  if (arguments.empty()) {
    parser::diagnostics::report_expected_type(context);
    return ParseResult<ast::GenericType *>::fail();
  }

  auto *type =
      context.get_ast().alloc<ast::GenericType>(
          name,
          std::move(arguments)
      );

  type->slice = name->slice;

  return ParseResult<ast::GenericType *>::ok(type);
}

ParseResult<ast::FunctionType *> Parser::parse_function_type() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::OPEN_PAREN)) { return ParseResult<ast::FunctionType *>::no_match(); }

  std::vector<ast::Type *> parameters;

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