#pragma once

#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/semantic/resolver/ContextStack.hpp"

namespace celestia::semantic {

struct SymbolCollectorContext {
  Compiler &compiler;
  CompilationUnit &unit;

  ContextStack<ScopeId> stack;

  CompilerEnvironment &env() const { return compiler.environment(); }

  const SourceSlice &diagnostic_slice() {

    if (auto *token = unit.tokens.current()) return token->slice;

    if (auto *token = unit.tokens.previous()) return token->slice;

    static SourceSlice empty{};
    return empty;
  }

  SymbolCollectorContext(Compiler &compiler, CompilationUnit &unit) : compiler(compiler), unit(unit), stack(ScopeId::invalid()) {}
};
} // namespace celestia::semantic