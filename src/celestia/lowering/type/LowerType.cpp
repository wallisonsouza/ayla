#include "celestia/lowering/Lowering.hpp"

namespace celestia::lowering {

ir::TypeId Lowering::lower_type(semantic::TypeId type_id) {

  auto &types = context.ir.get_global();

  if (!type_id.is_valid()) return ir::TypeId::invalid();

  const auto &type = context.compiler.environment().types.get(type_id);

  switch (type.kind) {

  case semantic::TypeKind::Primitive: {
    const auto &primitive = static_cast<const semantic::PrimitiveType &>(type);

    switch (primitive.primitive) {

    case semantic::PrimitiveKind::Void: return types.create_primitive_type(ir::TypeKind::Void);

    case semantic::PrimitiveKind::Bool: return types.create_primitive_type(ir::TypeKind::Bool);

    case semantic::PrimitiveKind::Char: return types.create_primitive_type(ir::TypeKind::Char);

    case semantic::PrimitiveKind::Int: return types.create_primitive_type(ir::TypeKind::Int);

    case semantic::PrimitiveKind::UInt: return types.create_primitive_type(ir::TypeKind::UInt);

    case semantic::PrimitiveKind::Int8: return types.create_primitive_type(ir::TypeKind::Int8);

    case semantic::PrimitiveKind::Int16: return types.create_primitive_type(ir::TypeKind::Int16);

    case semantic::PrimitiveKind::Int32: return types.create_primitive_type(ir::TypeKind::Int32);

    case semantic::PrimitiveKind::Int64: return types.create_primitive_type(ir::TypeKind::Int64);

    case semantic::PrimitiveKind::UInt8: return types.create_primitive_type(ir::TypeKind::UInt8);

    case semantic::PrimitiveKind::UInt16: return types.create_primitive_type(ir::TypeKind::UInt16);

    case semantic::PrimitiveKind::UInt32: return types.create_primitive_type(ir::TypeKind::UInt32);

    case semantic::PrimitiveKind::UInt64: return types.create_primitive_type(ir::TypeKind::UInt64);

    case semantic::PrimitiveKind::F32: return types.create_primitive_type(ir::TypeKind::F32);

    case semantic::PrimitiveKind::F64: return types.create_primitive_type(ir::TypeKind::F64);

    case semantic::PrimitiveKind::String: return types.create_primitive_type(ir::TypeKind::String);
    }

    break;
  }

  case semantic::TypeKind::Struct: {
    auto it = context.ir.get_program().type_map.find(type_id);

    if (it == context.ir.get_program().type_map.end()) return ir::TypeId::invalid();

    return it->second;
  }
  case semantic::TypeKind::Function: {
    const auto &function = static_cast<const semantic::FunctionType &>(type);

    std::vector<ir::TypeId> params;
    params.reserve(function.params.size());

    for (semantic::TypeId param : function.params) {
      ir::TypeId param_type = lower_type(param);

      if (!param_type.is_valid()) return ir::TypeId::invalid();

      params.push_back(param_type);
    }

    ir::TypeId return_type = lower_type(function.return_type);

    if (!return_type.is_valid()) return ir::TypeId::invalid();

    return types.create_function_type(std::move(params), return_type);
  }

  default: break;
  }

  return ir::TypeId::invalid();
}

} // namespace celestia::lowering