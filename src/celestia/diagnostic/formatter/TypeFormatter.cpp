#include "celestia/diagnostic/Formatter.hpp"

namespace diagnostic {

std::string DiagnosticFormatter::format_type(const CompilerEnvironment &env, celestia::semantic::TypeId id) {

  if (!id.is_valid()) { return "<invalid type>"; }

  const auto &type = env.types.get(id);

  switch (type.kind) {
  case celestia::semantic::TypeKind::Primitive: {
    const auto &primitive = static_cast<const celestia::semantic::PrimitiveType &>(type);

    return std::string(primitive_kind_name(primitive.primitive));
  }

  case celestia::semantic::TypeKind::Generic: {
    const auto &generic = static_cast<const celestia::semantic::GenericDeclarationType &>(type);

    const auto &symbol = env.symbols.get(generic.symbol);

    std::string result = symbol.name;

    if (generic.arity > 0) {
      result += "<";

      for (std::size_t i = 0; i < generic.arity; ++i) {
        if (i > 0) { result += ", "; }

        result += "?";
      }

      result += ">";
    }

    return result;
  }

  case celestia::semantic::TypeKind::GenericInstance: {
    const auto &generic = static_cast<const celestia::semantic::GenericInstanceType &>(type);

    const auto &constructor = env.types.get(generic.constructor);

    const auto &declaration = static_cast<const celestia::semantic::GenericDeclarationType &>(constructor);

    const auto &symbol = env.symbols.get(declaration.symbol);

    std::string result = symbol.name;

    if (!generic.arguments.empty()) {
      result += "<";

      for (std::size_t i = 0; i < generic.arguments.size(); ++i) {
        if (i > 0) { result += ", "; }

        result += format_type(env, generic.arguments[i]);
      }

      result += ">";
    }

    return result;
  }

  default: return type.to_string();
  }
}
} // namespace diagnostic