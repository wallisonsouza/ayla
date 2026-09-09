#include "celestia/ir/IRIds.hpp"
#include "celestia/transpiler/CGenerator.hpp"

namespace celestia::codegen {

void CGenerator::generate_type(ir::IRContext &context, const ir::Type &type) {

  switch (type.kind) {

  case ir::TypeKind::Int: out << "int"; break;

  case ir::TypeKind::Bool: out << "bool"; break;

  case ir::TypeKind::Struct: {
    const auto &struct_type = static_cast<const ir::StructType &>(type);

    ir::IRContext struct_context(context.get_program(), struct_type.module);

    const auto &structure = struct_context.get_struct(struct_type.id);

    out << "struct ";
    out << mangle_module_name(struct_context.get_global().get_string(struct_context.get_module().name));
    out << "_";
    out << struct_context.get_global().get_string(structure.name);

    break;
  }

    // ...
  }
}
void CGenerator::generate_struct(ir::IRContext &context, const ir::Struct &structure) {

  out << "struct ";

  out << mangle_module_name(context.get_global().get_string(context.get_module().name));
  out << "_";
  out << context.get_global().get_string(structure.name);

  out << " {\n";

  for (const auto &field : structure.fields) {

    out << "    ";

    generate_type(context, context.get_global().get_type(field.type));

    out << " ";

    out << context.get_global().get_string(field.name);

    out << ";\n";
  }

  out << "};\n";
}
} // namespace celestia::codegen