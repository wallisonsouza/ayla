#pragma once

#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/diagnostic/Diagnostic.hpp"

namespace celestia::syntax {
class ParseContext {
public:
  CompilationUnit &unit;
  CompilerEnvironment &env;

  explicit ParseContext(CompilationUnit &unit, CompilerEnvironment &e) : unit(unit), env(e) {}

  auto &tokens() { return unit.tokens; }


  auto &source() { return unit.source; }

  auto &get_ast() { return unit.arena; }

  auto &operators() { return env.language.operators; }

  auto &diagnostics() { return unit.diagnostics; }

  void report_error(diagnostic::Diagnostic diag) { unit.diagnostics.report(diag); }

  auto &descriptors() { return env.language.tokens; }

  const SourceSlice &diagnostic_slice() {

    if (auto *token = unit.tokens.current()) return token->slice;

    if (auto *token = unit.tokens.previous()) return token->slice;

    static SourceSlice empty{};
    return empty;
  }
};
} // namespace celestia::syntax