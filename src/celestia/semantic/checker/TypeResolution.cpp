

#include "celestia/ast/NodeCast.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::resolve_type(ast::Type *node) {
  assert(node);

  TypeId type = TypeId::invalid();

  switch (node->kind) {

  case ast::NodeKind::NamedType: type = resolve_named_type(ast::as<ast::NamedType>(node)); break;

  case ast::NodeKind::GenericType: type = resolve_generic_type(ast::as<ast::GenericType>(node)); break;

  default: std::cerr << "unsupported type resolution: " << ast::node_kind_name(node->kind) << '\n'; break;
  }

  if (type.is_valid()) context.unit.semantic.set_type(node, type);

  return type;
}

TypeId TypeChecker::resolve_pattern_type(ast::PatternNode *pattern) {
  assert(pattern);

  switch (pattern->kind) {

  case ast::NodeKind::NamedPattern: {
    auto *named = ast::as<ast::NamedPattern>(pattern);

    if (!named->type_annotation) return TypeId::invalid();

    return resolve_type(named->type_annotation);
  }

  default:
    return TypeId::invalid();
    // futuramente:
    // EnumVariantPattern
    // StructPattern
  }
}

TypeId TypeChecker::resolve_generic_type(ast::GenericType *node) {

  if (!node || !node->name) return TypeId::invalid();

  // Resolver already resolved the generic name.
  SymbolId symbol = context.unit.semantic.symbol(node);

  if (!symbol.is_valid()) {
    error(node, "invalid generic constructor symbol");
    return TypeId::invalid();
  }

  TypeId constructor = context.get_env().symbols.get(symbol).type;

  if (!constructor.is_valid()) {
    error(node, "generic constructor has no type");
    return TypeId::invalid();
  }

  const auto &constructor_type = context.get_env().types.get(constructor);

  if (constructor_type.kind != TypeKind::Generic) {
    error(node, "symbol is not a generic type");
    return TypeId::invalid();
  }

  const auto &generic = static_cast<const GenericDeclarationType &>(constructor_type);

  if (node->arguments.size() != generic.arity) {
    error(node, "wrong number of generic arguments");
    return TypeId::invalid();
  }

  std::vector<TypeId> arguments;

  for (auto *argument : node->arguments) {

    if (!argument) {
      error(node, "invalid generic argument");
      return TypeId::invalid();
    }

    TypeId argument_type = infer(argument);

    if (!argument_type.is_valid()) {
      error(argument, "invalid generic argument type");
      return TypeId::invalid();
    }

    arguments.push_back(argument_type);
  }

  TypeId type = context.get_env().types.get_or_create_generic_instance(constructor, arguments);

  if (!type.is_valid()) {
    error(node, "could not create generic instance");
    return TypeId::invalid();
  }
  return type;
}

TypeId TypeChecker::resolve_named_type(ast::NamedType *node) {

  if (!node) return TypeId::invalid();

  SymbolId symbol_id = context.unit.semantic.symbol(node);

  if (!symbol_id.is_valid()) {

    error(node, "named type symbol not found");
    return TypeId::invalid();
  }

  auto &symbol = context.get_env().symbols.get(symbol_id);

  if (!symbol.type.is_valid()) {

    error(node, "symbol has no valid type");
    return TypeId::invalid();
  }

  return symbol.type;
}

} // namespace celestia::semantic