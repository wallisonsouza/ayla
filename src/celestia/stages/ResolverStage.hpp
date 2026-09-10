#pragma once

#include "celestia/core/visitor/Stage.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

class ResolverStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    Resolver resolver = Resolver(compiler, unit);

    resolver.resolve_root(unit._root);
  }
};

} // namespace celestia::semantic
