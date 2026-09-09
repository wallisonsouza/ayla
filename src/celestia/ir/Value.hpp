#pragma once

#include <vector>

#include "celestia/ir/IRIds.hpp"
#include "celestia/ir/IRTable.hpp"
#include "celestia/ir/Instruction.hpp"

namespace celestia::ir {

struct Value {
  ValueId id;
  TypeId type;
};

struct StructField {
  StringId name;
  TypeId type;
};

struct Struct {
  StructId id;
  StringId name;
  std::vector<StructField> fields;
};

struct InterfaceMethod {
  StringId name;
  TypeId type;
};

struct Interface {
  InterfaceId id;
  StringId name;
  std::vector<InterfaceMethod> methods;
};

struct VTable {
  VTableId id;
  InterfaceId interface_id;
  StructId concrete_type;

  std::vector<FunctionId> methods;
};

struct Parameter {
  StringId name;
  ValueId value;
  TypeId type;
};

struct Function {
  FunctionId id;
  StringId name;

  TypeId return_type;

  BlockId entry;

  std::vector<Parameter> parameters;
  std::vector<BlockId> blocks;
};

class IRModule {
public:
  ModuleId id;
  StringId name;

  std::vector<ModuleId> imports;

  Table<Function, FunctionId> functions;
  Table<Struct, StructId> structs;
  Table<Interface, InterfaceId> interfaces;
  Table<VTable, VTableId> vtables;

  Table<Value, ValueId> values;
  Table<Instruction, InstructionId> instructions;
};

} // namespace celestia::ir
