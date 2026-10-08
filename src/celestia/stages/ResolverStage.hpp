#pragma once

#include "celestia/core/visitor/Stage.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

class ResolverStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    SemanticContext ctx(compiler, unit);

    Resolver resolver = Resolver(ctx);

    resolver.resolve_root(unit._root);
  }
};

} // namespace celestia::semantic
