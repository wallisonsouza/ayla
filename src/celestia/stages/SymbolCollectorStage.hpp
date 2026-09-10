#pragma once

#include "celestia/core/visitor/Stage.hpp"
#include "celestia/semantic/collector/SymbolCollector.hpp"

namespace celestia::semantic {

class SymbolCollectorStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    SymbolCollector collector(compiler, unit);

    collector.collect();
  }
};
} // namespace celestia::semantic