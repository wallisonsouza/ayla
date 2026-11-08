#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/NodeCast.hpp"
#include "celestia/ast/declaration/CapabilityDeclaration.hpp"
#include "celestia/ast/declaration/FunctionDeclaration.hpp"
#include "celestia/ast/declaration/ImplementationDeclaration.hpp"
#include "celestia/ast/declaration/ImportDeclaration.hpp"
#include "celestia/ast/declaration/StructDeclaration.hpp"
#include "celestia/ast/declaration/TypeDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"
#include "celestia/ast/names/Generic.hpp"
#include "celestia/semantic/resolver/Trace.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"




namespace celestia::syntax {

Match<std::vector<ast::GenericParameter *>> Parser::parse_generic_parameters() {

  auto &tokens = context.tokens();

  auto result = parse_delimited_list<ast::GenericParameter *>(context, TokenKind::LESS, TokenKind::GREATER, TokenKind::COMMA, [&]() -> Match<ast::GenericParameter *> {
    auto name_result = parse_identifier_name();

    if (name_result.is_error()) { return Match<ast::GenericParameter *>::fail(); }

    if (name_result.is_no_match()) {

      parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

      return Match<ast::GenericParameter *>::fail();
    }

    auto *name = name_result.value();

    std::vector<ast::Type *> constraints;

    tokens.skip_trivia();

    // T: {Printable, Drawable}
    if (tokens.match(TokenKind::COLON)) {

      tokens.skip_trivia();

      auto constraints_result = parse_delimited_list<ast::Type *>(context, TokenKind::OPEN_BRACE, TokenKind::CLOSE_BRACE, TokenKind::COMMA, [&]() -> Match<ast::Type *> { return parse_type(); });

      if (constraints_result.is_error()) { return Match<ast::GenericParameter *>::fail(); }

      if (constraints_result.is_no_match()) {

        parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

        return Match<ast::GenericParameter *>::fail();
      }

      constraints = std::move(constraints_result.value());
    }

    return Match<ast::GenericParameter *>::ok(context.get_ast().alloc<ast::GenericParameter>(name, std::move(constraints)));
  });

  if (result.is_error()) { return Match<std::vector<ast::GenericParameter *>>::fail(); }

  if (result.is_no_match()) { return Match<std::vector<ast::GenericParameter *>>::no_match(); }

  return Match<std::vector<ast::GenericParameter *>>::ok(std::move(result.value()));
}

Match<ast::Declaration *> Parser::parse_declaration() {

  auto specifiers = parse_specifiers();

  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::MODULE_KEYWORD: return upcast<ast::Declaration>(parse_module_declaration());

  case TokenKind::IMPORT_KEYWORD: return upcast<ast::Declaration>(parse_import_declaration());

  case TokenKind::IMPL_KEYWORD: return upcast<ast::Declaration>(parse_impl_declaration());

  case TokenKind::LET_KEYWORD:
  case TokenKind::MUT_KEYWORD:
  case TokenKind::CONST_KEYWORD: return upcast<ast::Declaration>(parse_variable_declaration(specifiers));

  default: break;
  }

  if (tokens.kind(1) != TokenKind::COLON) return Match<ast::Declaration *>::no_match();

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) { return Match<ast::Declaration *>::fail(); }

  if (name_result.is_no_match()) { return Match<ast::Declaration *>::no_match(); }

  auto *name = name_result.value();

  tokens.skip_trivia();

  if (!tokens.match(TokenKind::COLON)) {

    parser::diagnostics::report_expected(context, TokenKind::COLON);

    synchronize_declaration();

    return Match<ast::Declaration *>::fail();
  }

  tokens.skip_trivia();

  switch (tokens.kind()) {

  case TokenKind::FUN_KEYWORD: return upcast<ast::Declaration>(parse_function_declaration(name, specifiers));

  case TokenKind::CAP_KEYWORD: return upcast<ast::Declaration>(parse_capability_declaration(name, specifiers));

  case TokenKind::STRUCT_KEYWORD: return upcast<ast::Declaration>(parse_struct_declaration(name, specifiers));

  case TokenKind::TYPE_KEYWORD: return upcast<ast::Declaration>(parse_type_declaration(name, specifiers));

  case TokenKind::ENUM_KEYWORD: return upcast<ast::Declaration>(parse_enum_declaration(name, specifiers));

  default: synchronize_declaration(); return Match<ast::Declaration *>::fail();
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

// Enum variant
Match<ast::EnumVariant *> Parser::parse_enum_variant() {

  auto &tokens = context.tokens();

  tokens.skip_trivia();

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) { return Match<ast::EnumVariant *>::fail(); }

  if (name_result.is_no_match()) { return Match<ast::EnumVariant *>::no_match(); }

  auto *name = name_result.value();

  tokens.skip_trivia();

  std::vector<ast::Type *> arguments;

  if (tokens.check(TokenKind::OPEN_PAREN)) {

    auto arguments_result = parse_delimited_list<ast::Type *>(context, TokenKind::OPEN_PAREN, TokenKind::CLOSE_PAREN, TokenKind::COMMA, [&]() { return parse_type(); });

    if (arguments_result.is_error()) { return Match<ast::EnumVariant *>::fail(); }

    if (arguments_result.is_no_match()) { return Match<ast::EnumVariant *>::fail(); }

    arguments = std::move(arguments_result.value());
  }

  return Match<ast::EnumVariant *>::ok(context.get_ast().alloc<ast::EnumVariant>(name, std::move(arguments)));
}
// Enum declaration
Match<ast::EnumDeclaration *> Parser::parse_enum_declaration(ast::Identifier *name, DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  // enum
  if (!tokens.match(TokenKind::ENUM_KEYWORD)) { return Match<ast::EnumDeclaration *>::no_match(); }

  tokens.skip_trivia();

  // enum<T, U>
  auto generic_parameters_result = parse_generic_parameters();

  if (generic_parameters_result.is_error()) { return Match<ast::EnumDeclaration *>::fail(); }

  std::vector<ast::GenericParameter *> generic_parameters;

  if (!generic_parameters_result.is_no_match()) { generic_parameters = std::move(generic_parameters_result.value()); }

  tokens.skip_trivia();

  auto variants_result = parse_delimited_items<ast::EnumVariant *>(context, TokenKind::OPEN_BRACE, TokenKind::CLOSE_BRACE, [&]() { return parse_enum_variant(); });

  if (variants_result.is_error()) { return Match<ast::EnumDeclaration *>::fail(); }

  if (variants_result.is_no_match()) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return Match<ast::EnumDeclaration *>::fail();
  }

  return Match<ast::EnumDeclaration *>::ok(context.get_ast().alloc<ast::EnumDeclaration>(name, std::move(generic_parameters), std::move(variants_result.value()), specifiers));
}

// Module declaration
Match<ast::ModuleDeclaration *> Parser::parse_module_declaration() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::MODULE_KEYWORD)) { return Match<ast::ModuleDeclaration *>::no_match(); }

  auto name_result = parse_name();

  if (name_result.is_no_match()) {

    parser::diagnostics::report_expected_identifier(context);

    return Match<ast::ModuleDeclaration *>::fail();
  }

  if (name_result.is_error()) { return Match<ast::ModuleDeclaration *>::fail(); }

  celestia::debug::Trace::log(debug::Category::Parser, "parsed module declaration '{}'", name_result.value()->get_str());

  return Match<ast::ModuleDeclaration *>::ok(context.get_ast().alloc<ast::ModuleDeclaration>(name_result.value()));
}

// Import declaration
Match<ast::ImportDeclaration *> Parser::parse_import_declaration() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::IMPORT_KEYWORD)) { return Match<ast::ImportDeclaration *>::no_match(); }

  auto module_result = parse_name();

  if (!module_result.is_ok()) {

    parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

    return Match<ast::ImportDeclaration *>::fail();
  }

  std::optional<std::string> path;

  if (tokens.match(TokenKind::FROM_KEYWORD)) {

    tokens.skip_trivia();

    auto node = parse_string_literal();

    if (node->kind == ast::NodeKind::StringLiteral) {

      auto *string = ast::as<ast::StringLiteral>(node);

      if (node) { path = string->value; }
    }
  }

  celestia::debug::Trace::log(debug::Category::Parser, "parsed import declaration '{}'", module_result.value()->get_str());

  return Match<ast::ImportDeclaration *>::ok(context.get_ast().alloc<ast::ImportDeclaration>(module_result.value(), std::move(path)));
}

// Field declaration
Match<ast::FieldDeclaration *> Parser::parse_field_declaration() {

  auto &tokens = context.tokens();

  tokens.skip_trivia();

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) { return Match<ast::FieldDeclaration *>::fail(); }

  if (name_result.is_no_match()) { return Match<ast::FieldDeclaration *>::no_match(); }

  auto *identifier = name_result.value();

  if (!tokens.match(TokenKind::COLON)) {

    parser::diagnostics::report_expected(context, TokenKind::COLON);

    return Match<ast::FieldDeclaration *>::fail();
  }

  auto type_result = parse_type();

  if (type_result.is_error()) { return Match<ast::FieldDeclaration *>::fail(); }

  if (type_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return Match<ast::FieldDeclaration *>::fail();
  }

  return Match<ast::FieldDeclaration *>::ok(context.get_ast().alloc<ast::FieldDeclaration>(identifier, type_result.value()));
}

// Variable declaration
Match<ast::VariableDeclaration *> Parser::parse_variable_declaration(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  switch (tokens.kind()) {

  case TokenKind::LET_KEYWORD:
  case TokenKind::MUT_KEYWORD:
  case TokenKind::CONST_KEYWORD: tokens.consume(); break;

  default: return Match<ast::VariableDeclaration *>::no_match();
  }

  auto pattern_result = parse_pattern();

  if (pattern_result.is_error() || pattern_result.is_no_match()) { return Match<ast::VariableDeclaration *>::fail(); }

  ast::Expression *initializer = nullptr;

  if (tokens.match(TokenKind::ASSIGN)) {

    auto initializer_result = parse_expression();

    if (!initializer_result) {

      parser::diagnostics::report_expected_expression(context);

      return Match<ast::VariableDeclaration *>::fail();
    }

    initializer = initializer_result;
  }

  auto *node = context.get_ast().alloc<ast::VariableDeclaration>(pattern_result.value(), initializer, specifiers);

  node->slice = pattern_result.value()->slice;

  if (initializer) node->slice.extend_to(initializer->slice);

  return Match<ast::VariableDeclaration *>::ok(node);
}

// Capability member
Match<ast::Declaration *> Parser::parse_capability_member(DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  /*
   * Named members use:
   *
   * foo: fun(...)
   * T: type
   */

  auto name_result = parse_identifier_name();

  if (name_result.is_error()) { return Match<ast::Declaration *>::fail(); }

  if (name_result.is_no_match()) { return Match<ast::Declaration *>::no_match(); }

  auto *name = name_result.value();

  tokens.skip_trivia();

  if (!tokens.match(TokenKind::COLON)) {

    parser::diagnostics::report_expected(context, TokenKind::COLON);

    return Match<ast::Declaration *>::fail();
  }

  tokens.skip_trivia();

  switch (tokens.kind()) {

  case TokenKind::TYPE_KEYWORD: {

    auto result = parse_type_declaration(name, specifiers);

    if (result.is_error()) { return Match<ast::Declaration *>::fail(); }

    return Match<ast::Declaration *>::ok(result.value());
  }

  case TokenKind::FUN_KEYWORD: {

    auto result = parse_function_declaration(name, specifiers, false);

    if (result.is_error()) { return Match<ast::Declaration *>::fail(); }

    return Match<ast::Declaration *>::ok(result.value());
  }

  default: return Match<ast::Declaration *>::no_match();
  }
}

// Implementation declaration
Match<ast::ImplDeclaration *> Parser::parse_impl_declaration() {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::IMPL_KEYWORD)) { return Match<ast::ImplDeclaration *>::no_match(); }

  tokens.skip_trivia();

  // impl Iterable<Int>
  auto capability_result = parse_type();

  if (capability_result.is_error() || capability_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return Match<ast::ImplDeclaration *>::fail();
  }

  auto *capability = capability_result.value();

  tokens.skip_trivia();

  // for
  if (!tokens.match(TokenKind::FOR_KEYWORD)) {

    parser::diagnostics::report_expected(context, TokenKind::FOR_KEYWORD);

    return Match<ast::ImplDeclaration *>::fail();
  }

  tokens.skip_trivia();

  // Array<Int>
  auto target_result = parse_type();

  if (target_result.is_error() || target_result.is_no_match()) {

    parser::diagnostics::report_expected_type(context);

    return Match<ast::ImplDeclaration *>::fail();
  }

  auto *target = target_result.value();

  tokens.skip_trivia();

  // {
  if (!tokens.match(TokenKind::OPEN_BRACE)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return Match<ast::ImplDeclaration *>::fail();
  }

  std::vector<ast::FunctionDeclaration *> members;

  tokens.skip_trivia();

  bool closed = false;

  while (!tokens.is_end()) {

    tokens.skip_trivia();

    if (tokens.match(TokenKind::CLOSE_BRACE)) {

      closed = true;
      break;
    }

    /*
     * impl members:
     *
     * add: fun(a: Int) -> Int { ... }
     */

    auto name_result = parse_identifier_name();

    if (name_result.is_error()) { return Match<ast::ImplDeclaration *>::fail(); }

    if (name_result.is_no_match()) {

      parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

      return Match<ast::ImplDeclaration *>::fail();
    }

    auto *name = name_result.value();

    tokens.skip_trivia();

    if (!tokens.match(TokenKind::COLON)) {

      parser::diagnostics::report_expected(context, TokenKind::COLON);

      return Match<ast::ImplDeclaration *>::fail();
    }

    tokens.skip_trivia();

    auto function_result = parse_function_declaration(name, {}, true);

    if (function_result.is_error()) { return Match<ast::ImplDeclaration *>::fail(); }

    if (function_result.is_no_match()) {

      parser::diagnostics::report_expected(context, TokenKind::FUN_KEYWORD);

      return Match<ast::ImplDeclaration *>::fail();
    }

    members.push_back(function_result.value());

    tokens.skip_trivia();
  }

  if (!closed) {

    parser::diagnostics::report_expected(context, TokenKind::CLOSE_BRACE);

    return Match<ast::ImplDeclaration *>::fail();
  }

  return Match<ast::ImplDeclaration *>::ok(context.get_ast().alloc<ast::ImplDeclaration>(target, capability, std::move(members)));
}

// Capability declaration
Match<ast::CapabilityDeclaration *> Parser::parse_capability_declaration(ast::Identifier *name, DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  // cap
  if (!tokens.match(TokenKind::CAP_KEYWORD)) { return Match<ast::CapabilityDeclaration *>::no_match(); }

  tokens.skip_trivia();

  // cap<T, U>
  auto generic_parameters_result = parse_generic_parameters();

  // {
  if (!tokens.match(TokenKind::OPEN_BRACE)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return Match<ast::CapabilityDeclaration *>::fail();
  }

  debug::Trace::log(debug::Category::Parser, "capability body started");

  std::vector<ast::Declaration *> members;

  tokens.skip_trivia();

  bool closed = false;

  while (!tokens.is_end()) {

    tokens.skip_trivia();

    if (tokens.match(TokenKind::CLOSE_BRACE)) {

      closed = true;

      debug::Trace::log(debug::Category::Parser, "capability body ended");

      break;
    }

    auto member_result = parse_capability_member(specifiers);

    if (member_result.is_error()) { return Match<ast::CapabilityDeclaration *>::fail(); }

    if (member_result.is_no_match()) {

      parser::diagnostics::report_expected(context, TokenKind::FUN_KEYWORD);

      return Match<ast::CapabilityDeclaration *>::fail();
    }

    members.push_back(member_result.value());
  }

  if (!closed) {

    parser::diagnostics::report_expected(context, TokenKind::CLOSE_BRACE);

    return Match<ast::CapabilityDeclaration *>::fail();
  }

  debug::Trace::log(debug::Category::Parser, "creating CapabilityDeclaration with {} members", members.size());

  return Match<ast::CapabilityDeclaration *>::ok(context.get_ast().alloc<ast::CapabilityDeclaration>(name, std::move(generic_parameters_result.value()), std::move(members), specifiers));
}

// Type declaration
Match<ast::TypeDeclaration *> Parser::parse_type_declaration(ast::Identifier *name, DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  if (!tokens.match(TokenKind::TYPE_KEYWORD)) { return Match<ast::TypeDeclaration *>::no_match(); }

  tokens.skip_trivia();

  // type<T, U>
  auto generic_parameters_result = parse_generic_parameters();

  return Match<ast::TypeDeclaration *>::ok(context.get_ast().alloc<ast::TypeDeclaration>(name, std::move(generic_parameters_result.value()), specifiers));
}

// Struct declaration
Match<ast::StructDeclaration *> Parser::parse_struct_declaration(ast::Identifier *name, DeclarationSpecifiers specifiers) {

  auto &tokens = context.tokens();

  // struct
  if (!tokens.match(TokenKind::STRUCT_KEYWORD)) { return Match<ast::StructDeclaration *>::no_match(); }

  tokens.skip_trivia();

  // struct<T, U>
  auto generic_parameters_result = parse_generic_parameters();

  if (generic_parameters_result.is_error()) { return Match<ast::StructDeclaration *>::fail(); }

  std::vector<ast::GenericParameter *> generic_parameters;

  if (!generic_parameters_result.is_no_match()) { generic_parameters = std::move(generic_parameters_result.value()); }

  tokens.skip_trivia();

  std::vector<ast::Type *> compositions;

  // struct(Type, Type, ...)
  if (tokens.match(TokenKind::OPEN_PAREN)) {

    tokens.skip_trivia();

    // Empty composition list: struct()
    if (!tokens.match(TokenKind::CLOSE_PAREN)) {

      while (true) {

        auto type_result = parse_type();

        if (type_result.is_error()) { return Match<ast::StructDeclaration *>::fail(); }

        if (type_result.is_no_match()) {

          parser::diagnostics::report_expected_type(context);

          return Match<ast::StructDeclaration *>::fail();
        }

        compositions.push_back(type_result.value());

        tokens.skip_trivia();

        if (!tokens.match(TokenKind::COMMA)) { break; }

        tokens.skip_trivia();
      }

      if (!tokens.match(TokenKind::CLOSE_PAREN)) {

        parser::diagnostics::report_expected(context, TokenKind::CLOSE_PAREN);

        return Match<ast::StructDeclaration *>::fail();
      }
    }
  }

  tokens.skip_trivia();

  // {
  if (!tokens.match(TokenKind::OPEN_BRACE)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    return Match<ast::StructDeclaration *>::fail();
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

    if (field_result.is_error()) { return Match<ast::StructDeclaration *>::fail(); }

    if (field_result.is_no_match()) {

      parser::diagnostics::report_expected(context, diagnostic::ExpectedKind::Identifier);

      return Match<ast::StructDeclaration *>::fail();
    }

    fields.push_back(field_result.value());

    tokens.skip_trivia();
    tokens.match(TokenKind::COMMA);
  }

  if (!closed) {

    parser::diagnostics::report_expected(context, TokenKind::CLOSE_BRACE);

    return Match<ast::StructDeclaration *>::fail();
  }

  return Match<ast::StructDeclaration *>::ok(context.get_ast().alloc<ast::StructDeclaration>(name, std::move(generic_parameters), std::move(compositions), std::move(fields), specifiers));
}

// Function declaration
Match<ast::FunctionDeclaration *> Parser::parse_function_declaration(ast::Identifier *name, DeclarationSpecifiers specifiers, bool require_body) {

  auto &tokens = context.tokens();

  // fun
  if (!tokens.match(TokenKind::FUN_KEYWORD)) { return Match<ast::FunctionDeclaration *>::no_match(); }

  tokens.skip_trivia();

  // fun<T, U>
  auto generic_parameters_result = parse_generic_parameters();

  // Parameters
  if (!tokens.check(TokenKind::OPEN_PAREN)) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_PAREN);

    synchronize_declaration();

    return Match<ast::FunctionDeclaration *>::fail();
  }

  auto parameters = parse_delimited_list<ast::PatternNode *>(context, TokenKind::OPEN_PAREN, TokenKind::CLOSE_PAREN, TokenKind::COMMA, [&]() -> Match<ast::PatternNode *> {
    auto result = parse_pattern();

    if (result.is_error()) { return Match<ast::PatternNode *>::fail(); }

    if (result.is_no_match()) {

      parser::diagnostics::report_expected_pattern(context);

      return Match<ast::PatternNode *>::fail();
    }

    return result;
  });

  if (parameters.is_error()) {

    synchronize_declaration();

    return Match<ast::FunctionDeclaration *>::fail();
  }

  // -> Type
  ast::Type *return_type = nullptr;

  if (tokens.match(TokenKind::ARROW)) {

    auto result = parse_type();

    if (result.is_error()) {

      synchronize_declaration();

      return Match<ast::FunctionDeclaration *>::fail();
    }

    if (result.is_no_match()) {

      parser::diagnostics::report_expected_type(context);

      synchronize_declaration();

      return Match<ast::FunctionDeclaration *>::fail();
    }

    return_type = result.value();

  } else {

    auto result = speculate([&] { return parse_type(); });

    if (result.is_error()) { return Match<ast::FunctionDeclaration *>::fail(); }

    if (result.is_ok()) {

      parser::diagnostics::report_missing_return_arrow(context);

      synchronize_declaration();

      return Match<ast::FunctionDeclaration *>::fail();
    }
  }

  // { ... }
  ast::BlockExpression *body = nullptr;

  if (tokens.check(TokenKind::OPEN_BRACE)) {

    auto body_result = parse_block_expression();

    if (!body_result) {

      parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

      return Match<ast::FunctionDeclaration *>::fail();
    }

    body = body_result;

  } else if (require_body) {

    parser::diagnostics::report_expected(context, TokenKind::OPEN_BRACE);

    synchronize_declaration();

    return Match<ast::FunctionDeclaration *>::fail();
  }

  debug::Trace::log(debug::Category::Parser, "creating FunctionDeclaration");

  return Match<ast::FunctionDeclaration *>::ok(
      context.get_ast().alloc<ast::FunctionDeclaration>(name, std::move(generic_parameters_result.value()), std::move(parameters.value()), return_type, body, specifiers));
}

} // namespace celestia::syntax