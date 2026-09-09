#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

void Lowering::lower_function_declaration(const ast::FunctionDeclaration *node) {

  if (!node || !node->name) return;

  if (!node->return_type) return;

  auto return_type_id = context.unit.semantic.type(node->return_type);

  if (!return_type_id.is_valid()) return;

  ir::StringId name = context.global().intern_string(node->name->str);

  ir::TypeId return_type = lower_type(return_type_id);

  if (!return_type.is_valid()) return;

  ir::FunctionId function_id = context.ir.create_function(name, return_type);

  if (!function_id.is_valid()) return;

  for (const auto *parameter : node->parameters) {

    if (!parameter) continue;

    if (parameter->kind != ast::NodeKind::NamedPattern) continue;

    auto *p = static_cast<const ast::NamedPattern *>(parameter);

    auto parameter_type_id = context.unit.semantic.type(p);

    if (!parameter_type_id.is_valid()) continue;

    ir::StringId parameter_name = context.global().intern_string(p->name->str);

    ir::TypeId type = lower_type(parameter_type_id);

    if (!type.is_valid()) continue;

    ir::ValueId value = context.ir.create_value(type);

    if (!value.is_valid()) continue;

    context.ir.add_function_parameter(function_id, parameter_name, value);
  }
}

void Lowering::lower_module_declaration(const ast::ModuleDeclaration *node) {

  if (!node || !node->name) return;

  const auto module_name = node->name->get_str();

  const auto module_id = context.compiler.environment().modules.find(module_name);

  if (!module_id.is_valid()) return;

  auto &module = context.compiler.environment().modules.get(module_id);

  // --------------------------------------------------
  // Lower imports primeiro
  // --------------------------------------------------

  for (auto imported_id : module.imports()) {

    // if (!imported_id.is_valid()) continue;

    // auto &imported = context.compiler.environment().modules.get(imported_id);

    // auto *imported_unit = context.compiler.environment().units.get(imported.unit_);

    // if (!imported_unit) { throw std::runtime_error("module has no compilation unit: " + std::string(imported.name())); }

    // context.compiler.require(*imported_unit, stages::Lowering, CompilationRules::normal());
  }

  // --------------------------------------------------
  // Registra imports no módulo IR atual
  // --------------------------------------------------

  auto &ir_module = context.module();

  for (auto imported_id : module.imports()) {

    // if (!imported_id.is_valid()) continue;

    // auto &imported = context.compiler.environment().modules.get(imported_id);

    // auto *imported_unit = context.compiler.environment().units.get(imported.unit_);

    // if (!imported_unit) continue;

    // if (!imported_unit->ir_module.is_valid()) continue;

    // ir_module.imports.push_back(imported_unit->ir_module);
  }

  // --------------------------------------------------
  // Declarações locais
  // --------------------------------------------------

  for (const auto *declaration : node->declarations) {

    if (!declaration) continue;

    lower_declaration(declaration);
  }
}

} // namespace celestia::lowering