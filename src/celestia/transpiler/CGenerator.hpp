#pragma once

#include "celestia/ir/IRContext.hpp"
#include "celestia/ir/IRIds.hpp"
#include "celestia/ir/IRProgram.hpp"
#include "celestia/ir/Value.hpp"

#include <ostream>
#include <unordered_set>

namespace celestia::codegen {

class CGenerator {
public:
  CGenerator(std::ostream &out, ir::IRProgram &program) : out(out), program(program) {}

  void generate();

private:
  std::ostream &out;
  ir::IRProgram &program;
 std::unordered_set<ir::ModuleId> generated;
private:
  void generate_module(ir::IRContext &context);
  void generate_struct(ir::IRContext &context, const ir::Struct &structure);
  void generate_type(ir::IRContext &context, const ir::Type &type);
  void generate_value(ir::IRContext &context, const ir::Value &value);
  void generate_instruction(ir::IRContext &context, const ir::Instruction &instruction);
  void generate_function(ir::IRContext &context, const ir::Function &function);

  std::string mangle_module_name(std::string_view name) {
    std::string result(name);

    for (char &c : result) {
      if (c == '.') c = '_';
    }

    return result;
  }
};

} // namespace celestia::codegen