#pragma once

#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/ir/IRContext.hpp"

namespace celestia::lowering {

class LoweringContext {
public:
  ir::IRContext &ir;
  Compiler &compiler;
  CompilationUnit &unit;

  LoweringContext(ir::IRContext &ir, Compiler &compiler, CompilationUnit &unit) : ir(ir), compiler(compiler), unit(unit) {}

  ir::IRModule &module() { return ir.get_program().get_module(unit.ir_module); }

  const ir::IRModule &module() const { return ir.get_program().get_module(unit.ir_module); }

  ir::IRGlobalContext &global() { return ir.get_global(); }

  const ir::IRGlobalContext &global() const { return ir.get_global(); }

//   semantic::Environment &environment() { return compiler.environment(); }
};

} // namespace celestia::lowering