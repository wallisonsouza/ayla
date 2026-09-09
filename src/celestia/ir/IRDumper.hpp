// #pragma once

// #include <ostream>

// #include "celestia/ir/IR.hpp"

// namespace celestia::ir {

// class IRDumper {
// public:
//   explicit IRDumper(const IRContext &context) : context(context) {}

//   void dump(std::ostream &out) const;

// private:
//   const IRContext &context;

//   void dump_type(std::ostream &out, TypeId id) const;
//   void dump_value(std::ostream &out, ValueId id) const;
//   void dump_instruction(std::ostream &out, const Instruction &instruction) const;
//   void dump_struct(std::ostream &out, StructId id) const;
//    void dump_string(std::ostream &out, StringId id) const;
// };

// } // namespace celestia::ir