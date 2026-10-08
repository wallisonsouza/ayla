
#pragma once

#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/core/source/Source.hpp"
#include "celestia/diagnostic/Diagnostic.hpp"
#include "celestia/diagnostic/StyledText.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace diagnostic {

class DiagnosticFormatter {
public:
  // --------------------------------------------------
  // Public API: plain text
  // --------------------------------------------------

  static std::string format(std::string_view message, const Diagnostic &diagnostic, const CompilerEnvironment &env, const core::source::Source &source) {
    return format(message, diagnostic.arguments, env, source);
  }

  static std::string format(std::string_view message, LabelCode code, const std::vector<Argument> &arguments, const CompilerEnvironment &env, const core::source::Source &source) {
    std::string result(message);

    for (const auto &argument : arguments) { format_label_argument(result, code, argument, env, source); }

    return result;
  }

  static std::string format(std::string_view message, const std::vector<Argument> &arguments, const CompilerEnvironment &env, const core::source::Source &source) {
    std::string result(message);

    for (const auto &argument : arguments) { format_argument(result, argument, env, source); }

    return result;
  }

  // --------------------------------------------------
  // Public API: styled label text
  // --------------------------------------------------

  static StyledText format_styled(std::string_view message, LabelCode code, const std::vector<Argument> &arguments, const CompilerEnvironment &env, const core::source::Source &source) {
    StyledText result;

    append(result, std::string(message), TextRole::Normal);

    for (const auto &argument : arguments) { format_label_argument_styled(result, code, argument, env, source); }

    return result;
  }

private:
  static std::string format_type(const CompilerEnvironment &env, celestia::semantic::TypeId id);

  // --------------------------------------------------
  // Plain arguments
  // --------------------------------------------------

  static void format_argument(std::string &result, const Argument &argument, const CompilerEnvironment &env, const core::source::Source &source) {
    std::visit([&](const auto &value) { format_argument_impl(result, value, env, source); }, argument);
  }

  static void format_argument_impl(std::string &result, BinaryOperation operation, const CompilerEnvironment &, const core::source::Source &) {
    replace(result, "{operator}", format_binary_operation(operation));
  }

  static void format_argument_impl(std::string &result, const OperationTypes &types, const CompilerEnvironment &env, const core::source::Source &) {
    replace(result, "{lhs}", format_type(env, types.lhs));
    replace(result, "{rhs}", format_type(env, types.rhs));
  }

  template <typename T> static void format_argument_impl(std::string &result, const T &value, const CompilerEnvironment &env, const core::source::Source &source) {
    replace(result, "{value}", format_value(value, env, source));
  }

  // --------------------------------------------------
  // Plain label arguments
  // --------------------------------------------------

  static void format_label_argument(std::string &result, LabelCode code, const Argument &argument, const CompilerEnvironment &env, const core::source::Source &source) {
    std::visit([&](const auto &value) { format_label_argument_impl(result, code, value, env, source); }, argument);
  }

  static void format_label_argument_impl(std::string &result, LabelCode code, celestia::semantic::TypeId id, const CompilerEnvironment &env, const core::source::Source &) {
    const std::string type = format_type(env, id);

    switch (code) {
    case LabelCode::ExpectedType:
      replace(result, "{expected}", type);
      replace(result, "{type}", type);
      break;

    case LabelCode::FoundType:
      replace(result, "{found}", type);
      replace(result, "{type}", type);
      break;

    case LabelCode::LeftOperand:
      replace(result, "{left}", type);
      replace(result, "{type}", type);
      break;

    case LabelCode::RightOperand:
      replace(result, "{right}", type);
      replace(result, "{type}", type);
      break;

    default: break;
    }
  }

  template <typename T> static void format_label_argument_impl(std::string &result, LabelCode, const T &value, const CompilerEnvironment &env, const core::source::Source &source) {
    replace(result, "{value}", format_value(value, env, source));
  }

  // --------------------------------------------------
  // Styled label arguments
  // --------------------------------------------------

  static void format_label_argument_styled(StyledText &result, LabelCode code, const Argument &argument, const CompilerEnvironment &env, const core::source::Source &source) {
    std::visit([&](const auto &value) { format_label_argument_styled_impl(result, code, value, env, source); }, argument);
  }

  static void format_label_argument_styled_impl(StyledText &result, LabelCode code, celestia::semantic::TypeId id, const CompilerEnvironment &env, const core::source::Source &) {
    const std::string type = format_type(env, id);

    switch (code) {
    case LabelCode::ExpectedType:
      replace_styled(result, "{expected}", type, TextRole::Type);
      replace_styled(result, "{type}", type, TextRole::Type);
      break;

    case LabelCode::FoundType:
      replace_styled(result, "{found}", type, TextRole::Type);
      replace_styled(result, "{type}", type, TextRole::Type);
      break;

    case LabelCode::LeftOperand:
      replace_styled(result, "{left}", type, TextRole::Type);
      replace_styled(result, "{type}", type, TextRole::Type);
      break;

    case LabelCode::RightOperand:
      replace_styled(result, "{right}", type, TextRole::Type);
      replace_styled(result, "{type}", type, TextRole::Type);
      break;

    default: break;
    }
  }

  template <typename T> static void format_label_argument_styled_impl(StyledText &result, LabelCode, const T &value, const CompilerEnvironment &env, const core::source::Source &source) {
    replace_styled(result, "{value}", format_value(value, env, source), TextRole::Normal);
  }

  // --------------------------------------------------
  // Binary operation
  // --------------------------------------------------

  static std::string format_binary_operation(BinaryOperation operation) {
    switch (operation) {
    case BinaryOperation::Add: return "+";
    case BinaryOperation::Subtract: return "-";
    case BinaryOperation::Multiply: return "*";
    case BinaryOperation::Divide: return "/";
    case BinaryOperation::Modulo: return "%";

    case BinaryOperation::And: return "&&";
    case BinaryOperation::Or: return "||";

    case BinaryOperation::Equal: return "==";
    case BinaryOperation::NotEqual: return "!=";

    case BinaryOperation::Less: return "<";
    case BinaryOperation::LessEqual: return "<=";
    case BinaryOperation::Greater: return ">";
    case BinaryOperation::GreaterEqual: return ">=";

    case BinaryOperation::Assign: return "=";
    }

    return "<unknown operator>";
  }

  // --------------------------------------------------
  // Token
  // --------------------------------------------------

  static std::string format_value(Token *token, const CompilerEnvironment &, const core::source::Source &source) {
    if (!token) { return "unknown token"; }

    switch (token->kind()) {
    case TokenKind::NEW_LINE: return "end of statement";

    case TokenKind::EndOfFile: return "end of file";

    default: break;
    }

    const auto span = token->slice.get_span();
    const auto lexeme = source.buffer.get_view(span);

    return "'" + std::string(lexeme) + "'";
  }

  // --------------------------------------------------
  // TokenKind
  // --------------------------------------------------

  static std::string format_value(TokenKind kind, const CompilerEnvironment &env, const core::source::Source &) {
    auto *desc = env.language.tokens.lookup_by_kind(kind);

    if (!desc) { return "unknown token"; }

    return "'" + std::string(desc->name) + "'";
  }

  // --------------------------------------------------
  // Expected token
  // --------------------------------------------------

  static std::string format_value(const ExpectedToken &expected, const CompilerEnvironment &env, const core::source::Source &source) { return format_value(expected.kind, env, source); }

  // --------------------------------------------------
  // Expected category
  // --------------------------------------------------

  static std::string format_value(const ExpectedCategory &expected, const CompilerEnvironment &, const core::source::Source &) {
    switch (expected.kind) {
    case ExpectedKind::Identifier: return "an identifier";
    case ExpectedKind::Expression: return "an expression";
    case ExpectedKind::Statement: return "a statement";
    case ExpectedKind::Pattern: return "a pattern";
    case ExpectedKind::Type: return "a type";
    case ExpectedKind::Token: return "a token";
    case ExpectedKind::Declaration: return "a declaration";
    }

    std::unreachable();
  }

  // --------------------------------------------------
  // Type
  // --------------------------------------------------

  static std::string format_value(celestia::semantic::TypeId id, const CompilerEnvironment &env, const core::source::Source &) { return format_type(env, id); }

  // --------------------------------------------------
  // Symbol
  // --------------------------------------------------

  static std::string format_value(celestia::semantic::SymbolId id, const CompilerEnvironment &env, const core::source::Source &) {
    if (!id.is_valid()) { return "<invalid symbol>"; }

    const auto &symbol = env.symbols.get(id);

    return std::string(symbol_kind_name(symbol.kind)) + " '" + symbol.name + "'";
  }

  // --------------------------------------------------
  // Fallback
  // --------------------------------------------------

  template <typename T> static std::string format_value(const T &, const CompilerEnvironment &, const core::source::Source &) { return "<unknown value>"; }

  // --------------------------------------------------
  // Replace all occurrences: plain text
  // --------------------------------------------------

  static void replace(std::string &text, std::string_view from, std::string_view to) {
    if (from.empty()) { return; }

    std::size_t pos = 0;

    while ((pos = text.find(from, pos)) != std::string::npos) {
      text.replace(pos, from.size(), to);
      pos += to.size();
    }
  }
};

} // namespace diagnostic