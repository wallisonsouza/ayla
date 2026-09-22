#include "celestia/semantic/checker/TypeChecker.hpp"

namespace celestia::semantic {

void TypeChecker::check_function_declaration(ast::FunctionDeclaration *node) {

  if (!node) return;

  std::cout << "[TypeChecker] checking function\n";

  auto &semantic = context.unit.semantic;

  auto symbol_id = semantic.symbol(node);

  if (!symbol_id.is_valid()) {
    error(node, "function has invalid SymbolId");
    return;
  }

  auto &symbol = context.env().symbols.get(symbol_id);

  // FunctionType

  TypeId function_type_id = context.env().types.get_or_create(symbol_id, TypeKind::Function);

  if (!function_type_id.is_valid()) return;

  semantic.set_type(node, function_type_id);

  symbol.type = function_type_id;

  auto &type = context.env().types.get(function_type_id);

  auto &function_type = static_cast<FunctionType &>(type);

  // Parâmetros
  if (!check_function_parameters(node, function_type)) return;

  // Retorno
  if (!check_function_return_type(node, function_type)) return;

  std::cout << "[TypeChecker] function '" << symbol.name << "' type = " << function_type.to_string() << '\n';

  // Corpo
  check_function_body(node, function_type_id);
}


// Parameters
bool TypeChecker::check_function_parameters(ast::FunctionDeclaration *node, FunctionType &function_type) {

  for (auto *parameter : node->parameters) {

    if (!parameter) continue;

    TypeId parameter_type = check_pattern(parameter);

    if (!parameter_type.is_valid()) return false;

    function_type.params.push_back(parameter_type);
  }

  return true;
}

// Return
bool TypeChecker::check_function_return_type(ast::FunctionDeclaration *node, FunctionType &function_type) {

  if (node->return_type) {

    TypeId return_type = type_from_node(node->return_type);

    if (!return_type.is_valid()) {

      error(node, "invalid function return type");

      return false;
    }

    function_type.return_type = return_type;

    return true;
  }

  // TypeId void_type = context.env().builtins.void_type;

  // if (!void_type.is_valid()) {

  //   error(node, "void type is not initialized");

  //   return false;
  // }

  // function_type.return_type = void_type;

  return true;
}

// Body
void TypeChecker::check_function_body(ast::FunctionDeclaration *node, TypeId function_type) {

  TypeId previous_function = current_function;

  current_function = function_type;

  if (node->body) check(node->body);

  current_function = previous_function;
}

} // namespace celestia::semantic