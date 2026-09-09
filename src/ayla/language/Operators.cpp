#include "celestia/language/LanguageDefinition.hpp"

namespace ayla::language {

void register_operators(celestia::LanguageDefinition &def) {

  // prefix
  def.operators.add_prefix(TokenKind::NOT, 30, UnaryOperation::Not);

  // infix
  def.operators.add_infix(TokenKind::PLUS, 10, core::Associativity::Left, BinaryOperation::Add);

  def.operators.add_infix(TokenKind::MINUS, 10, core::Associativity::Left, BinaryOperation::Subtract);

  def.operators.add_infix(TokenKind::STAR, 20, core::Associativity::Left, BinaryOperation::Multiply);

  def.operators.add_infix(TokenKind::SLASH, 20, core::Associativity::Left, BinaryOperation::Divide);

  def.operators.add_infix(TokenKind::ASSIGN, 1, core::Associativity::Right, BinaryOperation::Assign);

  def.operators.add_infix(TokenKind::ARROW, 2, core::Associativity::Right, BinaryOperation::Arrow);

  def.operators.add_infix(TokenKind::EQUAL, 5, core::Associativity::Left, BinaryOperation::Equal);

  def.operators.add_infix(TokenKind::NOT_EQUAL, 5, core::Associativity::Left, BinaryOperation::NotEqual);

  def.operators.add_infix(TokenKind::LESS, 6, core::Associativity::Left, BinaryOperation::Less);

  def.operators.add_infix(TokenKind::LESS_EQUAL, 6, core::Associativity::Left, BinaryOperation::LessEqual);

  def.operators.add_infix(TokenKind::GREATER, 6, core::Associativity::Left, BinaryOperation::Greater);

  def.operators.add_infix(TokenKind::GREATER_EQUAL, 6, core::Associativity::Left, BinaryOperation::GreaterEqual);

  // postfix
  def.operators.add_postfix(TokenKind::OPEN_PAREN, 100, PostfixOperation::Call);

  def.operators.add_postfix(TokenKind::OPEN_BRACKET, 100, PostfixOperation::IndexAccess);

  def.operators.add_postfix(TokenKind::DOT, 100, PostfixOperation::MemberAccess);
}

} // namespace ayla::language