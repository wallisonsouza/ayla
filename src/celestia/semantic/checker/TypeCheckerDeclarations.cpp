#include "celestia/ast/declaration/FunctionDeclaration.hpp"
#include "celestia/ast/declaration/StructDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/semantic/checker/TypeChecker.hpp"
#include "celestia/semantic/checker/TypeCheckerDiagnostics.hpp"

namespace celestia::semantic {

TypeId TypeChecker::check_root(ast::RootNode *node) {
  assert(node);

  for (auto *module : node->modules) {
    if (!module) continue;

    if (!check(module).is_valid()) continue;
  }

  return context.get_env().builtins.void_type;
}

inline SourceSlice get_pattern_slice(ast::PatternNode *node) {

  switch (node->kind) {

  case celestia::ast::NodeKind::NamedPattern: {

    auto named = static_cast<ast::NamedPattern *>(node);

    return named->type_annotation ? named->type_annotation->slice : SourceSlice::EMPTY;
  }

  default: return SourceSlice::EMPTY;
  }
}

TypeId TypeChecker::check_variable_declaration(ast::VariableDeclaration *node) {
  assert(node);
  assert(node->pattern);

  TypeId declared_type = resolve_pattern_type(node->pattern);

  if (declared_type.is_valid()) {
    if (node->initializer) {
      ExpectedType expected{.type = declared_type, .node = node->pattern, .slice = get_pattern_slice(node->pattern)};

      TypeId initializer_type = check(node->initializer, expected);

      if (!initializer_type.is_valid()) { return TypeId::invalid(); }
    }

    context.unit.semantic.set_type(node->pattern, declared_type);
    context.unit.semantic.set_type(node, declared_type);

    return context.get_env().builtins.void_type;
  }

  if (!node->initializer) {
    checker::diagnostics::report_cannot_infer_type(context, node->slice);

    return TypeId::invalid();
  }

  TypeId initializer_type = check(node->initializer, ExpectedType::none());

  if (!initializer_type.is_valid()) { return TypeId::invalid(); }

  context.unit.semantic.set_type(node->pattern, initializer_type);
  context.unit.semantic.set_type(node, initializer_type);

  return context.get_env().builtins.void_type;
}

TypeId TypeChecker::check_module_declaration(ast::ModuleDeclaration *node) {
  if (!node) return TypeId::invalid();

  auto module_id = context.unit.semantic.module(node);

  if (!module_id.is_valid()) return TypeId::invalid();

  auto &module = context.get_env().modules.get(module_id);

  std::cout << "[TypeChecker] checking module: " << module.name() << '\n';

  for (auto *declaration : node->declarations) {
    if (!declaration) continue;

    if (!check(declaration).is_valid()) continue;
  }

  module.add_state(ModuleState::Checked);

  std::cout << "[TypeChecker] module checked\n";

  return context.get_env().builtins.void_type;
}

TypeId TypeChecker::check_import_declaration(ast::ImportDeclaration *node) {
  if (!node) return TypeId::invalid();

  // Futuramente valida imports aqui.

  return context.get_env().builtins.void_type;
}

TypeId TypeChecker::check_module_init_declaration(ast::ModuleInitDeclaration *node) {
  if (!node) return TypeId::invalid();

  std::cout << "[TypeChecker] checking module init\n";

  if (!node->body) return context.get_env().builtins.void_type;

  TypeId body_type = check(node->body);

  if (!body_type.is_valid()) return TypeId::invalid();

  return context.get_env().builtins.void_type;
}

TypeId TypeChecker::check_struct_declaration(ast::StructDeclaration *node) {
  if (!node) return TypeId::invalid();

  TypeId type = build_type(node);

  if (!type.is_valid()) return TypeId::invalid();

  return context.get_env().builtins.void_type;
}

TypeId TypeChecker::check_function_declaration(ast::FunctionDeclaration *node) {
  assert(node);

  TypeId function_type = build_type(node);

  if (!function_type.is_valid()) return TypeId::invalid();

  TypeId previous_function = current_function;

  current_function = function_type;

  if (node->body) {
    TypeId body_type = check(node->body);

    if (!body_type.is_valid()) {
      current_function = previous_function;
      return TypeId::invalid();
    }
  }

  current_function = previous_function;

  return context.get_env().builtins.void_type;
}

} // namespace celestia::semantic