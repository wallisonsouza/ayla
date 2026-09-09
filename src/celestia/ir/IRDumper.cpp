// #include "celestia/ir/IRDumper.hpp"

// #include <type_traits>

// namespace celestia::ir {

// void IRDumper::dump(std::ostream &out) const {

//   out << "=== IR ===\n\n";

//   out << "Types:\n";

//   for (uint32_t i = 0; i < context.type_count(); ++i) {

//     TypeId id{i};

//     out << "  %" << i << " = ";

//     dump_type(out, id);

//     out << '\n';
//   }

//   out << "\nStructs:\n";

//   for (uint32_t i = 0; i < context.struct_count(); ++i) {

//     StructId id{i};

//     out << "  %" << i << " = ";

//     dump_struct(out, id);

//     out << '\n';
//   }

//   out << "\nValues:\n";

//   for (uint32_t i = 0; i < context.value_count(); ++i) {

//     ValueId id{i};

//     const auto &value = context.get_value(id);

//     out << "  ";

//     dump_value(out, value.id);

//     out << " : ";

//     dump_type(out, value.type);

//     out << '\n';
//   }

//   out << "\nInstructions:\n";

//   for (uint32_t i = 0; i < context.instruction_count(); ++i) {

//     InstructionId id{i};

//     const auto &instruction = context.get_instruction(id);

//     out << "  ";

//     dump_instruction(out, instruction);

//     out << '\n';
//   }
// }

// void IRDumper::dump_type(std::ostream &out, TypeId id) const {

//   if (!id.is_valid()) {
//     out << "<invalid>";
//     return;
//   }

//   const Type &type = context.get_type(id);

//   switch (type.kind) {

//   case TypeKind::Void:
//   case TypeKind::Bool:
//   case TypeKind::Char:
//   case TypeKind::Int:
//   case TypeKind::UInt:
//   case TypeKind::Int8:
//   case TypeKind::Int16:
//   case TypeKind::Int32:
//   case TypeKind::Int64:
//   case TypeKind::UInt8:
//   case TypeKind::UInt16:
//   case TypeKind::UInt32:
//   case TypeKind::UInt64:
//   case TypeKind::F32:
//   case TypeKind::F64:
//   case TypeKind::String: out << type_kind_name(type.kind); break;

//   case TypeKind::Array: {

//     const auto &array = static_cast<const ArrayType &>(type);

//     out << "array<";

//     dump_type(out, array.element_type);

//     out << ">";

//     break;
//   }

//   case TypeKind::Pointer: {

//     const auto &pointer = static_cast<const PointerType &>(type);

//     out << "pointer<";

//     dump_type(out, pointer.pointee);

//     out << ">";

//     break;
//   }

//   case TypeKind::Struct: {

//     const auto &struct_type = static_cast<const StructType &>(type);

//     const Struct &structure = context.get_struct(struct_type.id);

//     dump_string(out, structure.name);

//     break;
//   }

//   case TypeKind::Function: out << "function"; break;
//   }
// }

// void IRDumper::dump_value(std::ostream &out, ValueId id) const {

//   if (!id.is_valid()) {
//     out << "<invalid>";
//     return;
//   }

//   out << "%" << id.index();
// }

// void IRDumper::dump_instruction(std::ostream &out, const Instruction &instruction) const {

//   if (instruction.result.is_valid()) {

//     dump_value(out, instruction.result);

//     out << " = ";
//   }

//   switch (instruction.kind) {

//   case InstructionKind::Constant: {

//     out << "constant ";

//     std::visit(
//         [&](const auto &value) {
//           using T = std::decay_t<decltype(value)>;

//           if constexpr (std::is_same_v<T, std::monostate>) {

//             out << "<none>";

//           } else if constexpr (std::is_same_v<T, std::string>) {

//             out << '"' << value << '"';

//           } else if constexpr (std::is_same_v<T, char>) {

//             out << '\'' << value << '\'';

//           } else {

//             out << value;
//           }
//         },
//         instruction.data);

//     break;
//   }

//   case InstructionKind::Add: out << "add "; break;

//   case InstructionKind::Sub: out << "sub "; break;

//   case InstructionKind::Mul: out << "mul "; break;

//   case InstructionKind::Div: out << "div "; break;

//   default: out << "<unknown>"; break;
//   }

//   for (size_t i = 0; i < instruction.operands.size(); ++i) {

//     if (i != 0) out << ", ";

//     dump_value(out, instruction.operands[i]);
//   }
// }

// void IRDumper::dump_struct(std::ostream &out, StructId id) const {

//   if (!id.is_valid()) {
//     out << "<invalid>";
//     return;
//   }

//   const Struct &structure = context.get_struct(id);

//   out << "struct ";

//   dump_string(out, structure.name);

//   out << " {\n";

//   for (const auto &field : structure.fields) {

//     out << "    ";

//     dump_string(out, field.name);

//     out << ": ";

//     dump_type(out, field.type);

//     out << '\n';
//   }

//   out << "  }";
// }

// void IRDumper::dump_string(std::ostream &out, StringId id) const {

//   if (!id.is_valid()) {
//     out << "<invalid>";
//     return;
//   }

//   out << context.get_string(id);
// }

// } // namespace celestia::ir