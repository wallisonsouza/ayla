#pragma once

#include <cstdint>
#include <string_view>

enum class TokenKind : uint8_t {
  STATIC,
  PUBLIC,
  PRIVATE,
  INVALID,
  COMMENT,

  EXTERN,
  MODULE_KEYWORD,
  ENUM_KEYWORD,
  MUT_KEYWORD,
  LET_KEYWORD,
  FOR_KEYWORD,
  CAP_KEYWORD,
  IMPL_KEYWORD,
  TYPE_KEYWORD,
  CONST_KEYWORD,
  FUN_KEYWORD,
  IMPORT_KEYWORD,
  FROM_KEYWORD,
  EXPORT_KEYWORD,
  STRUCT_KEYWORD,
  IF_KEYWORD,
  ELSE_KEYWORD,
  RETURN_KEYWORD,
  WHILE_KEYWORD,
  REF_KEYWORD,
  TRUE,
  FALSE,

  OPEN_BRACKET,
  CLOSE_BRACKET,

  COMMA,
  DOUBLE_QUOTE,
  SINGLE_QUOTE,
  COLON,
  ARROW,

  PLUS,
  NOT,
  MINUS,
  STAR,
  SLASH,

  ASSIGN,
  EQUAL,
  NOT_EQUAL,

  Ternary,

  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,

  OPEN_PAREN,
  Space,
  CLOSE_PAREN,

  OPEN_BRACE,
  CLOSE_BRACE,

  SEMI_COLON,
  DOT,

  Alias,
  IDENTIFIER,

  NUMBER_LITERAL,
  STRING_LITERAL,
  NULL_LITERAL,
  CHAR_LITERAL,

  EndOfFile,
  NEW_LINE
};

constexpr std::string_view token_kind_name(TokenKind kind) noexcept {

  switch (kind) {

  case TokenKind::STATIC: return "STATIC";
  case TokenKind::PUBLIC: return "PUBLIC";
  case TokenKind::PRIVATE: return "PRIVATE";
  case TokenKind::INVALID: return "INVALID";
  case TokenKind::COMMENT: return "COMMENT";

  case TokenKind::EXTERN: return "EXTERN";
  case TokenKind::MODULE_KEYWORD: return "MODULE_KEYWORD";
  case TokenKind::MUT_KEYWORD: return "MUT_KEYWORD";
  case TokenKind::FOR_KEYWORD: return "FOR_KEYWORD";
  case TokenKind::CAP_KEYWORD: return "CAP_KEYWORD";
  case TokenKind::IMPL_KEYWORD: return "IMPL_KEYWORD";
  case TokenKind::TYPE_KEYWORD: return "TYPE_KEYWORD";
  case TokenKind::CONST_KEYWORD: return "CONST_KEYWORD";
  case TokenKind::FUN_KEYWORD: return "FUNCTION_KEYWORD";
  case TokenKind::IMPORT_KEYWORD: return "IMPORT_KEYWORD";
  case TokenKind::STRUCT_KEYWORD: return "STRUCT_KEYWORD";

  case TokenKind::IF_KEYWORD: return "IF_KEYWORD";
  case TokenKind::ELSE_KEYWORD: return "ELSE_KEYWORD";
  case TokenKind::RETURN_KEYWORD: return "RETURN_KEYWORD";
  case TokenKind::WHILE_KEYWORD: return "WHILE_KEYWORD";

  case TokenKind::REF_KEYWORD: return "REF_KEYWORD";

  case TokenKind::TRUE: return "TRUE";
  case TokenKind::FALSE: return "FALSE";

  case TokenKind::OPEN_BRACKET: return "OPEN_BRACKET";
  case TokenKind::CLOSE_BRACKET: return "CLOSE_BRACKET";

  case TokenKind::COMMA: return "COMMA";
  case TokenKind::DOUBLE_QUOTE: return "DOUBLE_QUOTE";
  case TokenKind::SINGLE_QUOTE: return "SINGLE_QUOTE";
  case TokenKind::COLON: return "COLON";
  case TokenKind::ARROW: return "ARROW";

  case TokenKind::PLUS: return "PLUS";
  case TokenKind::NOT: return "NOT";
  case TokenKind::MINUS: return "MINUS";
  case TokenKind::STAR: return "STAR";
  case TokenKind::SLASH: return "SLASH";

  case TokenKind::ASSIGN: return "ASSIGN";
  case TokenKind::EQUAL: return "EQUAL";
  case TokenKind::NOT_EQUAL: return "NOT_EQUAL";

  case TokenKind::Ternary: return "TERNARY";

  case TokenKind::LESS: return "LESS";
  case TokenKind::LESS_EQUAL: return "LESS_EQUAL";
  case TokenKind::GREATER: return "GREATER";
  case TokenKind::GREATER_EQUAL: return "GREATER_EQUAL";

  case TokenKind::OPEN_PAREN: return "OPEN_PAREN";
  case TokenKind::CLOSE_PAREN: return "CLOSE_PAREN";

  case TokenKind::Space: return "SPACE";

  case TokenKind::OPEN_BRACE: return "OPEN_BRACE";
  case TokenKind::CLOSE_BRACE: return "CLOSE_BRACE";

  case TokenKind::SEMI_COLON: return "SEMI_COLON";
  case TokenKind::DOT: return "DOT";

  case TokenKind::Alias: return "ALIAS";

  case TokenKind::IDENTIFIER: return "IDENTIFIER";

  case TokenKind::NUMBER_LITERAL: return "NUMBER_LITERAL";
  case TokenKind::STRING_LITERAL: return "STRING_LITERAL";
  case TokenKind::NULL_LITERAL: return "NULL_LITERAL";
  case TokenKind::CHAR_LITERAL: return "CHAR_LITERAL";

  case TokenKind::EndOfFile: return "END_OF_FILE";
  case TokenKind::NEW_LINE: return "NEW_LINE";

  case TokenKind::EXPORT_KEYWORD: return "EXPORT_KEYWORD";
  case TokenKind::LET_KEYWORD: return "LET_KEYWORD";
  case TokenKind::FROM_KEYWORD: return "FROM_KEYWORD";
  case TokenKind::ENUM_KEYWORD: return "ENUM_KEYWORD";
  }

  return "UNKNOWN";
}