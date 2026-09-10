#pragma once

#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/syntax/parser/ParseStatus.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"
#include "celestia/syntax/parser/ParserDiagnostics.hpp"
#include <memory>

namespace celestia::syntax {

class ParseContext;

class ExpressionParser;

class Parser {

public:
  explicit Parser(ParseContext &context);
  ~Parser();

  void run();

  ast::RootNode *parse_root();
  ast::ModuleDeclaration *parse_explicit_module();
  void parse_module_body(ast::ModuleDeclaration *module);
  ast::ModuleDeclaration *create_implicit_module();
  DeclarationSpecifiers parse_specifiers();

  // names
  ParseResult<celestia::ast::IdentifierNode *> parse_identifier();
  // ParseResult<ast::QualifiedNameNode *> parse_qualified_name();
  ParseResult<ast::IdentifierNode *> parse_identifier_name();
  ParseResult<std::vector<ast::IdentifierNode *>> parse_generic_parameters();
  ParseResult<celestia::ast::NameNode *> parse_name();

  // types
  ParseResult<ast::TypeNode *> parse_type();

  ParseResult<ast::GenericTypeNode *> parse_generic_type(ast::NameNode *name);

  ParseResult<ast::NamedType *> parse_named_type();

  ParseResult<ast::FunctionType *> parse_function_type();

  // declarations
  ParseResult<ast::Declaration *> parse_declaration();

  ParseResult<ast::ModuleDeclaration *> parse_module_declaration();

  ParseResult<ast::ImportDeclaration *> parse_import_declaration();

  ParseResult<ast::VariableDeclaration *> parse_variable_declaration(DeclarationSpecifiers specifiers);

  ParseResult<ast::FunctionDeclaration *> parse_function_declaration(DeclarationSpecifiers specifiers, bool require_body);

  ParseResult<ast::CapabilityDeclaration *> parse_capability_declaration(DeclarationSpecifiers specifiers);

  ParseResult<ast::ImplDeclaration *> parse_impl_declaration(DeclarationSpecifiers specifiers);

  ParseResult<ast::StructDeclaration *> parse_struct_declaration(DeclarationSpecifiers specifiers);

  ParseResult<ast::TypeDeclaration *> parse_type_declaration(DeclarationSpecifiers specifiers);

  ParseResult<ast::Declaration *> named(DeclarationSpecifiers specifiers);

  ParseResult<ast::FieldDeclaration *> parse_field_declaration();

  // statements
  ast::Statement *parse_statement();

  ast::BlockStatement *parse_block_statement();

  ast::IfStatement *parse_if_statement();

  ast::WhileStatement *parse_while_statement();

  ast::ReturnStatement *parse_return_statement();

  ast::ExpressionStatement *parse_expression_statement();

  // patterns
  ParseResult<ast::PatternNode *> parse_pattern();
  ParseResult<ast::NamedPattern *> parse_named_pattern();

  static bool is_declaration_start(TokenKind kind) {
    switch (kind) {
    case TokenKind::FUN_KEYWORD:
    case TokenKind::STRUCT_KEYWORD:
    case TokenKind::CAP_KEYWORD:
    case TokenKind::IMPL_KEYWORD:
    case TokenKind::TYPE_KEYWORD:
    case TokenKind::MODULE_KEYWORD:
    case TokenKind::IMPORT_KEYWORD: return true;

    default: return false;
    }
  }

  static void synchronize_delimited_list(core::token::TokenStream &tokens, TokenKind open, TokenKind close, TokenKind separator) {

    int depth = 0;

    while (!tokens.is_end()) {
      const auto kind = tokens.kind();

      if (kind == open) {
        ++depth;
        tokens.consume();
        continue;
      }

      if (kind == close) {
        if (depth == 0) return;

        --depth;
        tokens.consume();
        continue;
      }

      if (depth == 0) {
        if (kind == separator) return;

        if (is_declaration_start(kind)) return;
      }

      tokens.consume();
    }
  }

  void synchronize_declaration() {

    auto &tokens = context.tokens();

    while (!tokens.is_end()) {
      const auto kind = tokens.kind();

      if (is_declaration_start(kind)) return;

      if (kind == TokenKind::CLOSE_BRACE) return;

      tokens.consume();
    }
  }

  template <typename Base, typename Derived> ParseResult<Base *> upcast(ParseResult<Derived *> result) {
    if (result.is_error()) return ParseResult<Base *>::fail();

    if (result.is_no_match()) return ParseResult<Base *>::no_match();

    return ParseResult<Base *>::ok(result.value());
  }

  template <typename T, typename Parser> ParseResult<std::vector<T>> parse_delimited_list(ParseContext &context, TokenKind open, TokenKind close, TokenKind separator, Parser &&parse_element) {

    auto &tokens = context.tokens();
    std::vector<T> elements;

    if (!tokens.match(open)) return ParseResult<std::vector<T>>::no_match();

    tokens.skip_trivia();

    if (tokens.match(close)) return ParseResult<std::vector<T>>::ok(std::move(elements));

    while (!tokens.is_end()) {

      auto result = parse_element();

      if (result.is_error()) return ParseResult<std::vector<T>>::fail();

      if (result.is_no_match()) return ParseResult<std::vector<T>>::fail();

      elements.push_back(result.value());

      tokens.skip_trivia();

      if (tokens.match(close)) return ParseResult<std::vector<T>>::ok(std::move(elements));

      if (!tokens.match(separator)) {

        parser::diagnostics::report_expected(context, separator);

        return ParseResult<std::vector<T>>::fail();
      }

      tokens.skip_trivia();

      if (tokens.match(close)) return ParseResult<std::vector<T>>::ok(std::move(elements));
    }

    parser::diagnostics::report_expected(context, close);

    return ParseResult<std::vector<T>>::fail();
  }

  ExpressionParser &expressions() { return *expression_parser; }

  template <typename Parser> auto speculate(Parser &&parser) {

    context.tokens().add_checkpoint();

    auto result = parser();

    context.tokens().rollback_checkpoint();

    return result;
  }

private:
  ParseContext &context;

  std::unique_ptr<ExpressionParser> expression_parser;
};

} // namespace celestia::syntax
