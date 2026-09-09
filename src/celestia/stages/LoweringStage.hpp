#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/core/visitor/Stage.hpp"
#include "celestia/ir/IRDumper.hpp"
#include "celestia/lowering/Lowering.hpp"

#include <iostream>

class LoweringStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    auto &program = compiler.program;

    if (!unit.ir_module.is_valid()) {

      // auto name = unit._root->name->get_str();

      // std:: cout << "name do modulo dessa merda: " << name;

      // auto string_id = program.get_global().intern_string(name);

      // unit.ir_module = program.create_module(string_id);
    }

    std::cerr << "[Lowering] unit.ir_module = " << unit.ir_module.index() << '\n';

    celestia::ir::IRContext ir(program, unit.ir_module);

    celestia::lowering::LoweringContext ctx(ir, compiler, unit);

    celestia::lowering::Lowering lowering(ctx);

    lowering.lower();
  }
};