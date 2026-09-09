#pragma once

#include "celestia/core/visitor/Stage.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

class SymbolCollectorStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    SymbolCollector collector(compiler, unit);

    collector.collect();
  }
};

class ResolverStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    Resolver resolver = Resolver(compiler, unit);

    resolver.resolve_root(unit._root);
  }
};

} // namespace celestia::semantic
