#include "celestia/compiler/CompilerEnvironment.hpp"

namespace celestia::semantic {
class TypeSystem {
public:
  explicit TypeSystem(CompilerEnvironment &env) : env(env) {}

  TypeId binary_result_type(BinaryOperation operation, TypeId lhs, TypeId rhs) const;

  TypeId common_type(TypeId lhs, TypeId rhs) const;

  bool is_same_type(TypeId lhs, TypeId rhs) const;

  bool is_assignable(TypeId target, TypeId source) const;

private:
  CompilerEnvironment &env;
};
} // namespace celestia::semantic
