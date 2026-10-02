#include "celestia/semantic/checker/TypeSystem.hpp"

namespace celestia::semantic {

bool TypeSystem::is_same_type(TypeId a, TypeId b) const {
  if (!a.is_valid() || !b.is_valid()) return false;

  return a == b;
}

bool TypeSystem::is_assignable(TypeId target, TypeId source) const {

  if (!target.is_valid() || !source.is_valid()) return false;

  if (is_same_type(target, source)) return true;

  const auto &target_type = env.types.get(target);
  const auto &source_type = env.types.get(source);

  if (target_type.kind == TypeKind::GenericInstance && source_type.kind == TypeKind::GenericInstance) {

    const auto &t = static_cast<const GenericInstanceType &>(target_type);

    const auto &s = static_cast<const GenericInstanceType &>(source_type);

    if (t.constructor != s.constructor) return false;

    if (t.arguments.size() != s.arguments.size()) return false;

    for (size_t i = 0; i < t.arguments.size(); ++i) {
      if (!is_assignable(t.arguments[i], s.arguments[i])) return false;
    }

    return true;
  }

  return false;
}

TypeId TypeSystem::binary_result_type(BinaryOperation operation, TypeId lhs, TypeId rhs) const {

  if (!lhs.is_valid() || !rhs.is_valid()) return TypeId::invalid();

  const TypeId bool_type = env.builtins.bool_type;

  switch (operation) {

  case BinaryOperation::Add:
  case BinaryOperation::Subtract:
  case BinaryOperation::Multiply:
  case BinaryOperation::Divide:
  case BinaryOperation::Modulo:

    if (!is_same_type(lhs, rhs)) return TypeId::invalid();

    return lhs;

  case BinaryOperation::And:
  case BinaryOperation::Or:

    if (!is_same_type(lhs, bool_type) || !is_same_type(rhs, bool_type)) return TypeId::invalid();

    return bool_type;

  case BinaryOperation::Equal:
  case BinaryOperation::NotEqual:
  case BinaryOperation::Less:
  case BinaryOperation::LessEqual:
  case BinaryOperation::Greater:
  case BinaryOperation::GreaterEqual:

    if (!is_same_type(lhs, rhs)) return TypeId::invalid();

    return bool_type;

  case BinaryOperation::Assign:
    if (!is_assignable(lhs, rhs)) return TypeId::invalid();

    return lhs;
  }

  return TypeId::invalid();
}

TypeId TypeSystem::common_type(TypeId lhs, TypeId rhs) const {

  if (!lhs.is_valid() || !rhs.is_valid()) return TypeId::invalid();

  if (is_same_type(lhs, rhs)) return lhs;

  if (is_assignable(lhs, rhs)) return lhs;

  if (is_assignable(rhs, lhs)) return rhs;

  return TypeId::invalid();
}

} // namespace celestia::semantic