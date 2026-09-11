#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/declarations/CapabilityDeclaration.hpp"
#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/ast/declarations/ImportDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/semantic/resolver/Trace.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

namespace celestia::syntax {

ParseResult<std::vector<ast::IdentifierNode *>> Parser::parse_generic_parameters() {

  auto result =
      parse_delimited_list<ast::IdentifierNode *>(context, TokenKind::LESS, TokenKind::GREATER, TokenKind::COMMA, [&]() -> ParseResult<ast::IdentifierNode *> { return parse_identifier_name(); });

  if (result.is_error()) return ParseResult<std::vector<ast::IdentifierNode *>>::fail();

  if (result.is_no_match()) return ParseResult<std::vector<ast::IdentifierNode *>>::no_match();

  return ParseResult<std::vector<ast::IdentifierNode *>>::ok(std::move(result.value()));
}

ParseResult<ast::Declaration *> Parser::parse_declaration() {

  auto specifiers = parse_specifiers();
  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::MODULE_KEYWORD: return upcast<ast::Declaration>(parse_module_declaration());

  case TokenKind::IMPORT_KEYWORD: return upcast<ast::Declaration>(parse_import_declaration());

  case TokenKind::FUN_KEYWORD: return upcast<ast::Declaration>(parse_function_declaration(specifiers, true));

  case TokenKind::CAP_KEYWORD: return upcast<ast::Declaration>(parse_capability_declaration(specifiers));

  case TokenKind::STRUCT_KEYWORD: return upcast<ast::Declaration>(parse_struct_declaration(specifiers));

  case TokenKind::IMPL_KEYWORD: return upcast<ast::Declaration>(parse_impl_declaration(specifiers));

  case TokenKind::TYPE_KEYWORD: return upcast<ast::Declaration>(parse_type_declaration(specifiers));

  case TokenKind::LET_KEYWORD:
  case TokenKind::MUT_KEYWORD:
  case TokenKind::CONST_KEYWORD: return upcast<ast::Declaration>(parse_variable_declaration(specifiers));

  default: return ParseResult<ast::Declaration *>::no_match();
  }
}

DeclarationSpecifiers Parser::parse_specifiers() {

  DeclarationSpecifiers specifiers;
  auto &tokens = context.tokens();

  while (Token *tok = tokens.current()) {

    switch (tok->kind()) {

    case TokenKind::PUBLIC:
      specifiers.visibility = Visibility::Public;
      tokens.consume();
      break;

    case TokenKind::PRIVATE:
      specifiers.visibility = Visibility::Private;
      tokens.consume();
      break;

    case TokenKind::STATIC:
      specifiers.modifiers.add(Modifier::Static);
      tokens.consume();
      break;

    case TokenKind::EXPORT_KEYWORD: tokens.consume(); break;

    case TokenKind::EXTERN:
      specifiers.modifiers.add(Modifier::Extern);
      tokens.consume();
      break;

    default: return specifiers;
    }
  }

  return specifiers;
}

// Module declaration
ParseResult<ast::ModuleDeclaration *> Parser::parse_module_declaration() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::MODULE_KEYWORD)) { return ParseResult<ast::ModuleDeclaration *>::no_match(); }

  auto name_result = parse_name();

  if (name_result.is_error()) { return ParseResult<ast::ModuleDeclaration *>::fail(); }

  if (name_result.is_no_match()) {

    parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

    return ParseResult<ast::ModuleDeclaration *>::fail();
  }

  celestia::debug::trace(debug::Category::Parser, "parsed module declaration '{}'", name_result.value()->get_str());

  return ParseResult<ast::ModuleDeclaration *>::ok(context.get_ast().alloc<ast::ModuleDeclaration>(name_result.value()));
}

// Import declaration
ParseResult<ast::ImportDeclaration *> Parser::parse_import_declaration() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::IMPORT_KEYWORD)) return ParseResult<ast::ImportDeclaration *>::no_match();

  auto module_result = parse_name();

  if (module_result.is_error()) return ParseResult<ast::ImportDeclaration *>::fail();

  if (module_result.is_no_match()) {

    parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

    return ParseResult<ast::ImportDeclaration *>::fail();
  }

  std::optional<std::string> path;

  if (tokens.match(TokenKind::FROM_KEYWORD)) {

    tokens.skip_trivia();

    auto node = parse_string_literal();

    if (node) { path = node->value; }
  }

  celestia::debug::trace(debug::Category::Parser, "parsed import declaration '{}'", module_result.value()->get_str());

  return ParseResult<ast::ImportDeclaration *>::ok(context.get_ast().alloc<ast::ImportDeclaration>(module_result.value(), std::move(path)));
}

// Field declaration
ParseResult<ast::FieldDeclaration *> Parser::parse_field_declaration() {

  auto &tokens = context.tokens();

  tokens.skip_trivia();

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) return ParseResult<ast::FieldDeclaration *>::fail();

  if (name_result.is_no_match()) return ParseResult<ast::FieldDeclaration *>::no_match();

  auto *identifier = name_result.value();

  if (!tokens.match(TokenKind::COLON)) {

    parser::diagnostics::report_expected(context, TokenKind::COLON);

    return ParseResult<ast::FieldDeclaration *>::fail();
  }

  auto type_result = parse_type();

  if (type_result.is_error()) return ParseResult<ast::FieldDeclaration *>::fail();

  if (type_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return ParseResult<ast::FieldDeclaration *>::fail();
  }

  return ParseResult<ast::FieldDeclaration *>::ok(context.get_ast().alloc<ast::FieldDeclaration>(identifier, type_result.value()));
}

// Variable declaration
ParseResult<ast::VariableDeclaration *> Parser::parse_variable_declaration(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::LET_KEYWORD:
  case TokenKind::MUT_KEYWORD:
  case TokenKind::CONST_KEYWORD: tokens.consume(); break;

  default: return ParseResult<ast::VariableDeclaration *>::no_match();
  }

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) return ParseResult<ast::VariableDeclaration *>::fail();

  if (name_result.is_no_match()) return ParseResult<ast::VariableDeclaration *>::no_match();

  auto *identifier = name_result.value();

  // let x:
  if (!tokens.match(TokenKind::COLON)) {

    parser::diagnostics::report_expected(context, TokenKind::COLON);

    return ParseResult<ast::VariableDeclaration *>::fail();
  }

  // let x: Int
  auto type_result = parse_type();

  if (type_result.is_error()) return ParseResult<ast::VariableDeclaration *>::fail();

  if (type_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return ParseResult<ast::VariableDeclaration *>::fail();
  }

  auto *type = type_result.value();

  ast::Expression *initializer = nullptr;

  if (tokens.match(TokenKind::ASSIGN)) {

    auto initializer_result = parse_expression();

    if (!initializer_result) {

      parser::diagnostics::report_expected_expression(context);

      return ParseResult<ast::VariableDeclaration *>::fail();
    }

    initializer = initializer_result;
  }

  auto *pattern = context.get_ast().alloc<ast::NamedPattern>(identifier, type);

  return ParseResult<ast::VariableDeclaration *>::ok(context.get_ast().alloc<ast::VariableDeclaration>(pattern, initializer, specifiers));
}

// Struct declaration
ParseResult<ast::StructDeclaration *> Parser::parse_struct_declaration(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::STRUCT_KEYWORD)) return ParseResult<ast::StructDeclaration *>::no_match();

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) return ParseResult<ast::StructDeclaration *>::fail();

  if (name_result.is_no_match()) return ParseResult<ast::StructDeclaration *>::no_match();

  auto *identifier = name_result.value();

  auto generic_parameters = parse_generic_parameters();

  if (generic_parameters.is_error()) return ParseResult<ast::StructDeclaration *>::fail();

  std::vector<ast::TypeNode *> compositions;

  // struct Name: Type, ...
  if (tokens.match(TokenKind::COLON)) {

    tokens.skip_trivia();

    while (true) {

      auto type_result = parse_type();

      if (type_result.is_error()) return ParseResult<ast::StructDeclaration *>::fail();

      if (type_result.is_no_match()) {

        parser::diagnostics::report_expected_type(context);

        return ParseResult<ast::StructDeclaration *>::fail();
      }

      compositions.push_back(type_result.value());

      tokens.skip_trivia();

      if (!tokens.match(TokenKind::COMMA)) break;

      tokens.skip_trivia();
    }
  }

  // require "{"
  if (!tokens.match(TokenKind::OPEN_BRACE)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return ParseResult<ast::StructDeclaration *>::fail();
  }

  std::vector<ast::FieldDeclaration *> fields;

  bool closed = false;

  while (!tokens.is_end()) {

    tokens.skip_trivia();

    if (tokens.match(TokenKind::CLOSE_BRACE)) {
      closed = true;
      break;
    }

    auto field_result = parse_field_declaration();

    if (field_result.is_error()) return ParseResult<ast::StructDeclaration *>::fail();

    if (field_result.is_no_match()) {
      parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

      return ParseResult<ast::StructDeclaration *>::fail();
    }

    fields.push_back(field_result.value());

    tokens.skip_trivia();
    tokens.match(TokenKind::COMMA);
  }

  if (!closed) {
    parser::diagnostics::report_expected(context, TokenKind::CLOSE_BRACE);

    return ParseResult<ast::StructDeclaration *>::fail();
  }

  std::vector<ast::IdentifierNode *> generics;

  if (!generic_parameters.is_no_match()) generics = std::move(generic_parameters.value());

  return ParseResult<ast::StructDeclaration *>::ok(context.get_ast().alloc<ast::StructDeclaration>(identifier, std::move(generics), std::move(compositions), std::move(fields), specifiers));
}

// Capability declaration
ParseResult<ast::CapabilityDeclaration *> Parser::parse_capability_declaration(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  debug::trace(debug::Category::Parser, "parsing capability declaration");

  if (!tokens.match(TokenKind::CAP_KEYWORD)) return ParseResult<ast::CapabilityDeclaration *>::no_match();

  // cap Add
  auto name_result = parse_identifier_name();

  if (name_result.is_error()) return ParseResult<ast::CapabilityDeclaration *>::fail();

  if (name_result.is_no_match()) return ParseResult<ast::CapabilityDeclaration *>::no_match();

  auto *identifier = name_result.value();

  auto generic_parameters = parse_generic_parameters();

  if (generic_parameters.is_error()) return ParseResult<ast::CapabilityDeclaration *>::fail();

  // {
  if (!tokens.match(TokenKind::OPEN_BRACE)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return ParseResult<ast::CapabilityDeclaration *>::fail();
  }

  debug::trace(debug::Category::Parser, "capability body started");

  std::vector<ast::FunctionDeclaration *> members;

  tokens.skip_trivia();

  while (!tokens.is_end()) {

    if (tokens.match(TokenKind::CLOSE_BRACE)) {

      debug::trace(debug::Category::Parser, "capability body ended");

      break;
    }

    auto function_result = parse_function_declaration(specifiers, false);

    if (function_result.is_error()) return ParseResult<ast::CapabilityDeclaration *>::fail();

    if (function_result.is_no_match()) {

      parser::diagnostics::report_expected(context, TokenKind::FUN_KEYWORD);

      return ParseResult<ast::CapabilityDeclaration *>::fail();
    }

    members.push_back(function_result.value());

    tokens.skip_trivia();
  }

  if (tokens.is_end()) {

    parser::diagnostics::report_expected(context, TokenKind::CLOSE_BRACE);

    return ParseResult<ast::CapabilityDeclaration *>::fail();
  }

  debug::trace(debug::Category::Parser, "creating CapabilityDeclaration with {} members", members.size());

  std::vector<ast::IdentifierNode *> generics;

  if (!generic_parameters.is_no_match()) generics = std::move(generic_parameters.value());

  return ParseResult<ast::CapabilityDeclaration *>::ok(context.get_ast().alloc<ast::CapabilityDeclaration>(identifier, std::move(generics), std::move(members), specifiers));
}

// Implementation declaration
ParseResult<ast::ImplDeclaration *> Parser::parse_impl_declaration(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::IMPL_KEYWORD)) return ParseResult<ast::ImplDeclaration *>::no_match();

  // impl Int
  auto target_name_result = parse_name();

  if (target_name_result.is_error()) return ParseResult<ast::ImplDeclaration *>::fail();

  if (target_name_result.is_no_match()) {

    parser::diagnostics::report_expected_identifier(context);

    return ParseResult<ast::ImplDeclaration *>::fail();
  }

  auto *target_name = target_name_result.value();

  auto *target = context.get_ast().alloc<ast::NamedType>(target_name);

  auto generic_parameters = parse_generic_parameters();

  if (generic_parameters.is_error()) return ParseResult<ast::ImplDeclaration *>::fail();

  // impl Int: Add
  auto capability_result = parse_type();

  if (capability_result.is_error()) return ParseResult<ast::ImplDeclaration *>::fail();

  if (capability_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return ParseResult<ast::ImplDeclaration *>::fail();
  }

  auto *capability = capability_result.value();

  // {
  if (!tokens.match(TokenKind::OPEN_BRACE)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return ParseResult<ast::ImplDeclaration *>::fail();
  }

  std::vector<ast::FunctionDeclaration *> members;

  tokens.skip_trivia();

  while (!tokens.is_end()) {

    if (tokens.match(TokenKind::CLOSE_BRACE)) break;

    auto function_result = parse_function_declaration(specifiers, true);

    if (function_result.is_error()) return ParseResult<ast::ImplDeclaration *>::fail();

    if (function_result.is_no_match()) {

      parser::diagnostics::report_expected(context, TokenKind::FUN_KEYWORD);

      return ParseResult<ast::ImplDeclaration *>::fail();
    }

    members.push_back(function_result.value());

    tokens.skip_trivia();
  }

  if (tokens.is_end()) {

    parser::diagnostics::report_expected(context, TokenKind::CLOSE_BRACE);

    return ParseResult<ast::ImplDeclaration *>::fail();
  }

  std::vector<ast::IdentifierNode *> generics;

  if (!generic_parameters.is_no_match()) generics = std::move(generic_parameters.value());

  return ParseResult<ast::ImplDeclaration *>::ok(context.get_ast().alloc<ast::ImplDeclaration>(std::move(generics), target, capability, std::move(members)));
}

// Type declaration
ParseResult<ast::TypeDeclaration *> Parser::parse_type_declaration(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::TYPE_KEYWORD)) return ParseResult<ast::TypeDeclaration *>::no_match();

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) return ParseResult<ast::TypeDeclaration *>::fail();

  if (name_result.is_no_match()) {

    parser::diagnostics::report_expected_identifier(context);

    return ParseResult<ast::TypeDeclaration *>::fail();
  }
  auto generic_parameters = parse_generic_parameters();

  if (generic_parameters.is_error()) return ParseResult<ast::TypeDeclaration *>::fail();

  std::vector<ast::IdentifierNode *> generics;

  if (!generic_parameters.is_no_match()) generics = std::move(generic_parameters.value());

  return ParseResult<ast::TypeDeclaration *>::ok(context.get_ast().alloc<ast::TypeDeclaration>(name_result.value(), std::move(generics), specifiers));
}

// Function declaration
ParseResult<ast::FunctionDeclaration *> Parser::parse_function_declaration(DeclarationSpecifiers specifiers, bool require_body) {

  auto &tokens = context.tokens();

  debug::trace(debug::Category::Parser, "parsing function declaration");

  if (!tokens.match(TokenKind::FUN_KEYWORD)) return ParseResult<ast::FunctionDeclaration *>::no_match();

  debug::trace(debug::Category::Parser, "function keyword found");

  // fun name
  auto name_result = parse_identifier_name();

  if (name_result.is_error()) {

    synchronize_declaration();

    return ParseResult<ast::FunctionDeclaration *>::fail();
  }

  if (name_result.is_no_match()) {

    synchronize_declaration();

    return ParseResult<ast::FunctionDeclaration *>::fail();
  }

  auto *identifier = name_result.value();

  // fun name<T, U>
  auto generic_parameters = parse_generic_parameters();

  if (generic_parameters.is_error()) {

    debug::trace(debug::Category::Parser, "error in generic parameters");

    return ParseResult<ast::FunctionDeclaration *>::fail();
  }

  // (
  if (!tokens.check(TokenKind::OPEN_PAREN)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_PAREN);

    synchronize_declaration();

    return ParseResult<ast::FunctionDeclaration *>::fail();
  }

  auto parameters = parse_delimited_list<ast::PatternNode *>(context, TokenKind::OPEN_PAREN, TokenKind::CLOSE_PAREN, TokenKind::COMMA, [&]() -> ParseResult<ast::PatternNode *> {
    auto result = parse_pattern();

    if (result.is_error()) return ParseResult<ast::PatternNode *>::fail();

    if (result.is_no_match()) {

      parser::diagnostics::report_expected_pattern(context);

      return ParseResult<ast::PatternNode *>::fail();
    }

    return result;
  });

  if (parameters.is_error()) {

    synchronize_declaration();

    return ParseResult<ast::FunctionDeclaration *>::fail();
  }

  // -> Type
  ast::TypeNode *return_type = nullptr;

  if (tokens.match(TokenKind::ARROW)) {

    auto result = parse_type();

    if (result.is_error()) {

      synchronize_declaration();

      return ParseResult<ast::FunctionDeclaration *>::fail();
    }

    if (result.is_no_match()) {

      parser::diagnostics::report_expected_type(context);

      synchronize_declaration();

      return ParseResult<ast::FunctionDeclaration *>::fail();
    }

    return_type = result.value();

  } else {

    // Detect:
    //
    // fun add(a: Int, b: Int) Int {
    //
    // instead of producing the less useful:
    //
    // expected '{', found 'Int'

    auto result = speculate([&] { return parse_type(); });

    if (result.is_error()) { return ParseResult<ast::FunctionDeclaration *>::fail(); }

    if (result.is_ok()) {

      parser::diagnostics::report_missing_return_arrow(context);

      synchronize_declaration();

      return ParseResult<ast::FunctionDeclaration *>::fail();
    }
  }

  // { ... }
  ast::BlockStatement *body = nullptr;

  if (tokens.check(TokenKind::OPEN_BRACE)) {

    //  auto body_result = parse_block_statement();

    // if (body_result.is_error()) return ParseResult<ast::FunctionDeclaration *>::fail();

    // if (body_result.is_no_match()) {

    //   parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    //   return ParseResult<ast::FunctionDeclaration *>::fail();
    // }

    // body = body_result.value();

    auto body_result = parse_block_statement();

    if (!body_result) {

      parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

      return ParseResult<ast::FunctionDeclaration *>::fail();
    }

    body = body_result;

  } else if (require_body) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    synchronize_declaration();

    return ParseResult<ast::FunctionDeclaration *>::fail();
  }

  debug::trace(debug::Category::Parser, "creating FunctionDeclaration");

  std::vector<ast::IdentifierNode *> generics;

  if (!generic_parameters.is_no_match()) generics = std::move(generic_parameters.value());

  return ParseResult<ast::FunctionDeclaration *>::ok(context.get_ast().alloc<ast::FunctionDeclaration>(identifier, std::move(generics), std::move(parameters.value()), return_type, body, specifiers));
}

} // namespace celestia::syntax
