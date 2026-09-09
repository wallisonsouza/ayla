#pragma once

#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/core/source/Source.hpp"
#include "celestia/diagnostic/Diagnostic.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace diagnostic {

class DiagnosticFormatter {

public:
  // Substitui os placeholders pela representação textual dos argumentos.
  static std::string format(std::string_view message, const std::vector<DiagnosticArgument> &arguments, const CompilerEnvironment &env, const core::source::Source &source) {

    std::string result(message);

    for (const auto &argument : arguments) {

      const auto placeholder = placeholder_name(argument.kind);
      const auto value = format_value(argument.value, env, source);

      replace(result, placeholder, value);
    }

    return result;
  }

private:
  // --------------------------------------------------
  // Placeholder
  // --------------------------------------------------

  static std::string_view placeholder_name(DiagnosticArgumentKind kind) {

    switch (kind) {

    case DiagnosticArgumentKind::Expected: return "{expected}";

    case DiagnosticArgumentKind::Found: return "{found}";

    case DiagnosticArgumentKind::Previous: return "{previous}";

    case DiagnosticArgumentKind::Type: return "{type}";

    case DiagnosticArgumentKind::Symbol: return "{symbol}";

    case DiagnosticArgumentKind::Name: return "{name}";
    }

    return "{unknown}";
  }

  // --------------------------------------------------
  // Value
  // --------------------------------------------------

  static std::string format_value(const DiagnosticValue &value, const CompilerEnvironment &env, const core::source::Source &source) {

    return std::visit([&](const auto &value) -> std::string { return format_value_impl(value, env, source); }, value);
  }

  // --------------------------------------------------
  // Token
  // --------------------------------------------------

  static std::string format_value_impl(Token *token, const CompilerEnvironment &, const core::source::Source &source) {

    if (!token) return "unknown token";

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

  static std::string format_value_impl(TokenKind kind, const CompilerEnvironment &env, const core::source::Source &) {

    auto *desc = env.language.tokens.lookup_by_kind(kind);

    if (!desc) return "unknown token";

    return "'" + std::string(desc->name) + "'";
  }

  // --------------------------------------------------
  // Expected token
  // --------------------------------------------------

  static std::string format_value_impl(const ExpectedToken &expected, const CompilerEnvironment &env, const core::source::Source &source) { return format_value_impl(expected.kind, env, source); }

  // --------------------------------------------------
  // Expected category
  // --------------------------------------------------

  static std::string format_value_impl(const ExpectedCategory &expected, const CompilerEnvironment &, const core::source::Source &) {

    switch (expected.kind) {

    case ExpectedKind::Identifier: return "an identifier";

    case ExpectedKind::Expression: return "an expression";

    case ExpectedKind::Statement: return "a statement";

    case ExpectedKind::Pattern: return "a pattern";

    case ExpectedKind::Type: return "a type";
    }

    std::unreachable();
  }

  // --------------------------------------------------
  // Type
  // --------------------------------------------------

  static std::string format_value_impl(celestia::semantic::TypeId id, const CompilerEnvironment &env, const core::source::Source &) {

    if (!id.is_valid()) return "<invalid type>";

    const auto &type = env.types.get(id);

    return type.to_string();
  }

  // --------------------------------------------------
  // Symbol
  // --------------------------------------------------

  static std::string format_value_impl(celestia::semantic::SymbolId id, const CompilerEnvironment &env, const core::source::Source &) {

    if (!id.is_valid()) return "<invalid symbol>";

    const auto &symbol = env.symbols.get(id);


    return symbol.name;
  }

  // --------------------------------------------------
  // String
  // --------------------------------------------------

  static std::string format_value_impl(const std::string &value, const CompilerEnvironment &, const core::source::Source &) { return value; }

  // --------------------------------------------------
  // Replace
  // --------------------------------------------------

  static void replace(std::string &text, std::string_view from, std::string_view to) {

    std::size_t pos = 0;

    while ((pos = text.find(from, pos)) != std::string::npos) {

      text.replace(pos, from.size(), to);

      pos += to.size();
    }
  }
};

} // namespace diagnostic