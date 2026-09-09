#include "celestia/core/token/TokenKind.hpp"
#include "celestia/language/LanguageDefinition.hpp"
namespace ayla::language {
void register_tokens(celestia::LanguageDefinition &def) {

  // keywords
  def.tokens.add(TokenKind::IDENTIFIER, TokenGroup::Name);
  def.tokens.add(TokenKind::TRUE, "true", TokenGroup::Keyword);
  def.tokens.add(TokenKind::FALSE, "false", TokenGroup::Keyword);

  def.tokens.add(TokenKind::IF_KEYWORD, "if", TokenGroup::Keyword);
  def.tokens.add(TokenKind::EXTERN, "extern", TokenGroup::Keyword);
  def.tokens.add(TokenKind::ELSE_KEYWORD, "else", TokenGroup::Keyword);
  def.tokens.add(TokenKind::WHILE_KEYWORD, "while", TokenGroup::Keyword);
  def.tokens.add(TokenKind::RETURN_KEYWORD, "return", TokenGroup::Keyword);

  // decl
  def.tokens.add(TokenKind::FUN_KEYWORD, "fun", TokenGroup::Keyword);
  def.tokens.add(TokenKind::STRUCT_KEYWORD, "struct", TokenGroup::Keyword);
  def.tokens.add(TokenKind::TYPE_KEYWORD, "type", TokenGroup::Keyword);
  def.tokens.add(TokenKind::CAP_KEYWORD, "cap", TokenGroup::Keyword);
  def.tokens.add(TokenKind::IMPL_KEYWORD, "impl", TokenGroup::Keyword);

  def.tokens.add(TokenKind::PUBLIC, "public", TokenGroup::Keyword);
  def.tokens.add(TokenKind::PRIVATE, "private", TokenGroup::Keyword);

  // var
  def.tokens.add(TokenKind::STATIC, "static", TokenGroup::Keyword);
  def.tokens.add(TokenKind::MUT_KEYWORD, "mut", TokenGroup::Keyword);
  def.tokens.add(TokenKind::LET_KEYWORD, "let", TokenGroup::Keyword);
  def.tokens.add(TokenKind::CONST_KEYWORD, "const", TokenGroup::Keyword);

  def.tokens.add(TokenKind::MODULE_KEYWORD, "module", TokenGroup::Keyword);
  def.tokens.add(TokenKind::IMPORT_KEYWORD, "import", TokenGroup::Keyword);
  def.tokens.add(TokenKind::EXPORT_KEYWORD, "export", TokenGroup::Keyword);

  // punctuaction
  def.tokens.add(TokenKind::OPEN_PAREN, "(", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::COLON, ":", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::DOT, ".", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::CLOSE_PAREN, ")", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::OPEN_BRACE, "{", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::CLOSE_BRACE, "}", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::OPEN_BRACKET, "[", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::CLOSE_BRACKET, "]", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::SEMI_COLON, ";", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::COMMA, ",", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::SINGLE_QUOTE, "\'", TokenGroup::Punctuation);
  def.tokens.add(TokenKind::NEW_LINE, "\\n", TokenGroup::Whitespace);

  // literals
  def.tokens.add(TokenKind::NUMBER_LITERAL, TokenGroup::Literal);
  def.tokens.add(TokenKind::STRING_LITERAL, TokenGroup::Literal);
  def.tokens.add(TokenKind::CHAR_LITERAL, TokenGroup::Literal);
  def.tokens.add(TokenKind::NULL_LITERAL, TokenGroup::Literal);

  // operators

  def.tokens.add(TokenKind::PLUS, "+", TokenGroup::Operator);
  def.tokens.add(TokenKind::MINUS, "-", TokenGroup::Operator);
  def.tokens.add(TokenKind::STAR, "*", TokenGroup::Operator);
  def.tokens.add(TokenKind::SLASH, "/", TokenGroup::Operator);

  def.tokens.add(TokenKind::ASSIGN, "=", TokenGroup::Operator);
  def.tokens.add(TokenKind::ARROW, "->", TokenGroup::Operator);

  def.tokens.add(TokenKind::EQUAL, "==", TokenGroup::Operator);
  def.tokens.add(TokenKind::NOT_EQUAL, "!=", TokenGroup::Operator);

  def.tokens.add(TokenKind::LESS, "<", TokenGroup::Operator);
  def.tokens.add(TokenKind::LESS_EQUAL, "<=", TokenGroup::Operator);
  def.tokens.add(TokenKind::GREATER, ">", TokenGroup::Operator);
  def.tokens.add(TokenKind::GREATER_EQUAL, ">=", TokenGroup::Operator);

  def.tokens.add(TokenKind::NOT, "!", TokenGroup::Operator);
}
} // namespace ayla::language