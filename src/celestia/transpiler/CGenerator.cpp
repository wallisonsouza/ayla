#include "CGenerator.hpp"

#include "celestia/ir/IRContext.hpp"
#include <unordered_set>

namespace celestia::codegen {

void CGenerator::generate() {

  std::unordered_set<ir::ModuleId> generated;

  for (const auto &module : program.get_modules()) {

    ir::IRContext context(program, module->id);

    generate_module(context);
  }

  
}

void CGenerator::generate_module(ir::IRContext &context) {

  const auto &module = context.get_module();

  if (!generated.insert(module.id).second) return;

  for (ir::ModuleId import : module.imports) {

    ir::IRContext import_ctx(context.get_program(), import);

    generate_module(import_ctx);
  }

  for (auto &structure : module.structs) { generate_struct(context, structure); }
}

void CGenerator::generate_function(ir::IRContext &context, const ir::Function &function) {

  generate_type(context, context.get_global().get_type(function.return_type));

  out << " ";

  out << context.get_global().get_string(context.get_module().name);
  out << "_";
  out << context.get_global().get_string(function.name);

  out << "(";

  for (size_t i = 0; i < function.parameters.size(); ++i) {

    if (i > 0) out << ", ";

    const auto &parameter = function.parameters[i];

    generate_type(context, context.get_global().get_type(parameter.type));

    out << " ";

    out << context.get_global().get_string(parameter.name);

    out << " ";
  }

  out << ")";

  out << " {\n";

  // corpo

  out << "}\n";
}

} // namespace celestia::codegen