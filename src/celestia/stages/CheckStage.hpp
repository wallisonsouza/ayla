#pragma once

#include "celestia/core/visitor/Stage.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

class CheckStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    TypeCheckerContext ctx(compiler, unit);

    TypeChecker checker(ctx);

    checker.check(unit._root);
  }
};

} // namespace celestia::semantic