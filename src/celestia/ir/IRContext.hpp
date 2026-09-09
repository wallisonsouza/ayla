#pragma once

#include <utility>
#include <vector>

#include "celestia/ir/IRGlobal.hpp"
#include "celestia/ir/IRProgram.hpp"
#include "celestia/ir/Value.hpp"
namespace celestia::ir {

class IRContext {
private:
  IRProgram &program;
  ModuleId module_id;

public:
  IRContext(IRProgram &program, ModuleId module_id) : program(program), module_id(module_id) {}

  // ==================================================
  // Context
  // ==================================================

  IRProgram &get_program() { return program; }

  IRGlobalContext &get_global() { return program.get_global(); }

  IRModule &get_module() { return program.get_module(module_id); }

  // ==================================================
  // Module
  // ==================================================

  void add_module_import(ModuleId imported_module) {

    if (!imported_module.is_valid()) return;

    get_module().imports.push_back(imported_module);
  }

  // ==================================================
  // Function
  // ==================================================

  FunctionId create_function(StringId name, TypeId return_type) {

    if (!name.is_valid() || !return_type.is_valid()) return FunctionId::invalid();

    return get_module().functions.create([&](FunctionId id) {
      return Function{
          .id = id,
          .name = name,
          .return_type = return_type,
          .entry = BlockId::invalid(),
          .parameters = {},
          .blocks = {},
      };
    });
  }

  void add_function_parameter(FunctionId function_id, StringId name, ValueId value) {

    if (!function_id.is_valid() || !name.is_valid() || !value.is_valid()) return;

    auto &function = get_module().functions.get(function_id);

    function.parameters.push_back({
        .name = name,
        .value = value,
        .type = get_value(value).type,
    });
  }

  // ==================================================
  // Struct
  // ==================================================

  StructId create_struct(StringId name) {

    if (!name.is_valid()) return StructId::invalid();

    return get_module().structs.create([&](StructId id) {
      return Struct{
          .id = id,
          .name = name,
      };
    });
  }

  void add_struct_field(StructId struct_id, StringId name, TypeId type) {

    if (!struct_id.is_valid() || !name.is_valid() || !type.is_valid()) return;

    get_module().structs.get(struct_id).fields.push_back({
        .name = name,
        .type = type,
    });
  }

  // ==================================================
  // Interface
  // ==================================================

  InterfaceId create_interface(StringId name) {

    if (!name.is_valid()) return InterfaceId::invalid();

    return get_module().interfaces.create([&](InterfaceId id) {
      return Interface{
          .id = id,
          .name = name,
      };
    });
  }

  void add_interface_method(InterfaceId interface_id, StringId name, TypeId type) {

    if (!interface_id.is_valid() || !name.is_valid() || !type.is_valid()) return;

    get_module()
        .interfaces.get(interface_id)
        .methods.push_back({
            .name = name,
            .type = type,
        });
  }

  // ==================================================
  // VTable
  // ==================================================

  VTableId create_vtable(InterfaceId interface_id, StructId concrete_type) {

    if (!interface_id.is_valid() || !concrete_type.is_valid()) return VTableId::invalid();

    return get_module().vtables.create([&](VTableId id) {
      return VTable{
          .id = id,
          .interface_id = interface_id,
          .concrete_type = concrete_type,
          .methods = {},
      };
    });
  }

  void add_vtable_method(VTableId vtable_id, FunctionId function_id) {

    if (!vtable_id.is_valid() || !function_id.is_valid()) return;

    get_module().vtables.get(vtable_id).methods.push_back(function_id);
  }

  // ==================================================
  // Values
  // ==================================================

  ValueId create_value(TypeId type) {

    if (!type.is_valid()) return ValueId::invalid();

    return get_module().values.create([&](ValueId id) {
      return Value{
          .id = id,
          .type = type,
      };
    });
  }

  template <typename T> ValueId create_constant(TypeId type, T value) {

    ValueId result = create_value(type);

    if (!result.is_valid()) return ValueId::invalid();

    get_module().instructions.create([&](InstructionId id) {
      return Instruction{
          .id = id,
          .kind = InstructionKind::Constant,
          .result = result,
          .data = std::move(value),
      };
    });

    return result;
  }

  Value &get_value(ValueId id) { return get_module().values.get(id); }

  Instruction &get_instruction(InstructionId id) { return get_module().instructions.get(id); }

  Function &get_function(FunctionId id) { return get_module().functions.get(id); }

  Struct &get_struct(StructId id) { return get_module().structs.get(id); }

  Interface &get_interface(InterfaceId id) { return get_module().interfaces.get(id); }

  VTable &get_vtable(VTableId id) { return get_module().vtables.get(id); }

  // ==================================================
  // Counts
  // ==================================================

  size_t struct_count() { return get_module().structs.size(); }

  size_t function_count() { return get_module().functions.size(); }

  size_t interface_count() { return get_module().interfaces.size(); }

  size_t vtable_count() { return get_module().vtables.size(); }

  size_t value_count() { return get_module().values.size(); }

  size_t instruction_count() { return get_module().instructions.size(); }
};

} // namespace celestia::ir